#include "cryptominder.h"
#include "receive.h"
#include "send.h"
#include "addwallet.h"
#include "deletewallet.h"
#include <QString>
#include <QRegularExpression>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include "ui/ui_cryptominder.h"
#include "statistics.h"
#include <QMessageBox>

Cryptominder::Cryptominder(QWidget *parent)
    : QMainWindow(parent)
    , BaseDatabase()
    , ui(new Ui::Cryptominder)
{
    ui->setupUi(this);
    create_account("Mikalai", "test", "123");
    account->load_from_db(1);
    auto rows = account->get_wallets_from_db(1);
    for (const auto &row : rows) {
        QString wallet_address = QString::fromStdString(row["wallet_address"].c_str());
        ui->comboBox->addItem(wallet_address);
    }
    Wallet *wallet = nullptr;
    QString label = ui->comboBox->currentText();
    std::string wallet_address = label.toStdString();
    if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
        double balance = wallet->get_wallet_balance();
        QString wallet_balance = QString::number(balance, 'f', 2);
        ui->money_label->setText(wallet_balance + " TON");
    }
    populate_transaction_list();
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::activated), this, [this](int index) {
        Wallet *wallet = nullptr;
        auto wallet_address = ui->comboBox->itemText(index).toStdString();
        if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
            double balance = wallet->get_wallet_balance();
            QString wallet_balance = QString::number(balance, 'f', 2);
            ui->money_label->setText(wallet_balance + " TON");
        }
        populate_transaction_list();
    });
}

Cryptominder::~Cryptominder() = default;

void Cryptominder::on_receive_button_clicked()
{
    try {
        if (!is_database_connected()) {
            throw DatabaseException("Database connection is broken.");
        }
        auto receive_window = std::make_unique<Receive>(this);
        connect(this, &Cryptominder::send_data, receive_window.get(), &Receive::receive_data);
        QString walletAddress = ui->comboBox->currentText();
        emit send_data(walletAddress);
        receive_window->setModal(true);
        receive_window->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_send_button_clicked()
{
    try {
        if (!is_database_connected()) {
            throw DatabaseException("Database connection is broken.");
        }
        QString label_text = ui->comboBox->currentText();
        auto send_window = std::make_unique<Send>(this);
        connect(this, &Cryptominder::send_data, send_window.get(), &Send::receive_data);
        emit send_data(label_text);
        connect(send_window.get(), &Send::send_money, this, &Cryptominder::update_wallet_data);
        connect(send_window.get(), &Send::send_money, this, &Cryptominder::populate_transaction_list);
        send_window->setModal(true);
        send_window->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_delete_wallet_clicked()
{
    try {
        if (!is_database_connected()) {
            throw DatabaseException("Database connection is broken.");
        }
        auto delete_wallet = std::make_unique<DeleteWallet>(this);
        QString label_text = ui->comboBox->currentText();
        connect(this, &Cryptominder::send_data, delete_wallet.get(), &DeleteWallet::receive_data);
        connect(delete_wallet.get(), &DeleteWallet::wallet_deleted, this, &Cryptominder::update_wallet_data);
        connect(delete_wallet.get(), &DeleteWallet::wallet_deleted, this, &Cryptominder::populate_transaction_list);
        emit send_data(label_text);
        delete_wallet->setModal(true);
        delete_wallet->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_top_up_button_clicked()
{
    try {
        if (!is_database_connected()) {
            throw DatabaseException("Database connection is broken.");
        }

        Wallet *wallet = nullptr;
        QString label_text = ui->comboBox->currentText();
        std::string wallet_address = label_text.toStdString();

        if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
            *wallet + 50;
            double balance = wallet->get_wallet_balance();
            QString wallet_balance = QString::number(balance, 'f', 2);
            ui->money_label->setText(wallet_balance + " TON");
        }
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

bool Cryptominder::is_database_connected()
{
    try {
        BaseDatabase database;
        pqxx::connection* conn = database.getConnection();
        return conn->is_open();
    } catch (const pqxx::broken_connection &) {
        return false;
    }
}

void Cryptominder::update_wallet_data() {
    ui->comboBox->clear();
    account->load_from_db(1);
    auto rows = account->get_wallets_from_db(1);
    for (const auto &row : rows) {
        QString wallet_address = QString::fromStdString(row["wallet_address"].c_str());
        ui->comboBox->addItem(wallet_address);
    }
    Wallet *wallet = nullptr;
    auto wallet_address = ui->comboBox->currentText().toStdString();
    if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
        double balance = wallet->get_wallet_balance();
        QString wallet_balance = QString::number(balance, 'f', 2);
        ui->money_label->setText(wallet_balance + " TON");
    } else {
        ui->money_label->setText("? TON");
    }
}

void Cryptominder::populate_transaction_list() {
    ui->transactionTable->clear();
    QString label = ui->comboBox->currentText();
    std::string wallet_address = label.toStdString();

    BaseDatabase database;
    pqxx::connection* conn = database.getConnection();

    Transaction transaction(*conn);

    auto transactions = transaction.get_transactions(wallet_address);

    ui->transactionTable->setRowCount(static_cast<int>(transactions.size()));
    ui->transactionTable->setColumnCount(3);
    ui->transactionTable->setHorizontalHeaderLabels({"From", "To", "Amount"});
    for (int i = 0; i < transactions.size(); ++i) {
        const auto &t = transactions[i];
        ui->transactionTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(t.sender_wallet_address)));
        ui->transactionTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(t.receiver_wallet_address)));
        ui->transactionTable->setItem(i, 2, new QTableWidgetItem(QString::number(t.amount, 'f', 2)));
    }
}

void Cryptominder::on_get_statistics_clicked()
{
    try {
        if (!is_database_connected()) {
            throw DatabaseException("Database connection is broken.");
        }
        QString label_text = ui->comboBox->currentText();
        auto statistics = std::make_unique<Statistics>(this);
        connect(this, &Cryptominder::send_data, statistics.get(), &Statistics::receive_data);
        emit send_data(label_text);
        statistics->setModal(true);
        statistics->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_add_wallet_clicked()
{
    try {
        if (!is_database_connected()) {
            throw DatabaseException("Database connection is broken.");
        }
        auto add_wallet = std::make_unique<AddWallet>(this);
        connect(add_wallet.get(), &AddWallet::wallet_added, this, &Cryptominder::update_wallet_data);
        connect(add_wallet.get(), &AddWallet::wallet_added, this, &Cryptominder::populate_transaction_list);
        add_wallet->setModal(true);
        add_wallet->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
