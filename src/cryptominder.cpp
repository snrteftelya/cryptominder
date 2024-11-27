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
#include "DataBaseException.h"
#include <QMessageBox>

Cryptominder::Cryptominder(QWidget *parent)
    : QMainWindow(parent)
    , BaseDatabase()
    , ui(new Ui::Cryptominder)
{
    ui->setupUi(this);
    createAccount("Mikalai", "test", "123");
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
    populateTransactionList();
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::activated), this, [this](int index) {
        Wallet *wallet = nullptr;
        auto wallet_address = ui->comboBox->itemText(index).toStdString();
        if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
            double balance = wallet->get_wallet_balance();
            QString wallet_balance = QString::number(balance, 'f', 2);
            ui->money_label->setText(wallet_balance + " TON");
        }
        populateTransactionList();
    });
}

Cryptominder::~Cryptominder() = default;

void Cryptominder::on_receive_button_clicked()
{
    try {
        if (!isDatabaseConnected()) {
            throw DatabaseException("Database connection is broken.");
        }
        auto receive_window = std::make_unique<Receive>(this);
        connect(this, &Cryptominder::sendData, receive_window.get(), &Receive::receiveData);
        QString walletAddress = ui->comboBox->currentText();
        emit sendData(walletAddress);
        receive_window->setModal(true);
        receive_window->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_send_button_clicked()
{
    try {
        if (!isDatabaseConnected()) {
            throw DatabaseException("Database connection is broken.");
        }
        QString label_text = ui->comboBox->currentText();
        auto send_window = std::make_unique<Send>(this);
        connect(this, &Cryptominder::sendData, send_window.get(), &Send::receiveData);
        emit sendData(label_text);
        connect(send_window.get(), &Send::send_money, this, &Cryptominder::updateWalletData);
        connect(send_window.get(), &Send::send_money, this, &Cryptominder::populateTransactionList);
        send_window->setModal(true);
        send_window->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_delete_wallet_clicked()
{
    try {
        if (!isDatabaseConnected()) {
            throw DatabaseException("Database connection is broken.");
        }
        auto delete_wallet = std::make_unique<DeleteWallet>(this);
        QString label_text = ui->comboBox->currentText();
        connect(this, &Cryptominder::sendData, delete_wallet.get(), &DeleteWallet::receiveData);
        connect(delete_wallet.get(), &DeleteWallet::walletDeleted, this, &Cryptominder::updateWalletData);
        connect(delete_wallet.get(), &DeleteWallet::walletDeleted, this, &Cryptominder::populateTransactionList);
        emit sendData(label_text);
        delete_wallet->setModal(true);
        delete_wallet->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_top_up_button_clicked()
{
    try {
        if (!isDatabaseConnected()) {
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

bool Cryptominder::isDatabaseConnected()
{
    try {
        BaseDatabase database;
        pqxx::connection* conn = database.getConnection();
        return conn->is_open();
    } catch (const pqxx::broken_connection &) {
        return false;
    }
}

void Cryptominder::updateWalletData() {
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

void Cryptominder::populateTransactionList() {
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
        if (!isDatabaseConnected()) {
            throw DatabaseException("Database connection is broken.");
        }
        QString label_text = ui->comboBox->currentText();
        auto statistics = std::make_unique<Statistics>(this);
        connect(this, &Cryptominder::sendData, statistics.get(), &Statistics::receiveData);
        emit sendData(label_text);
        statistics->setModal(true);
        statistics->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void Cryptominder::on_add_wallet_clicked()
{
    try {
        if (!isDatabaseConnected()) {
            throw DatabaseException("Database connection is broken.");
        }
        auto add_wallet = std::make_unique<AddWallet>(this);
        connect(add_wallet.get(), &AddWallet::walletAdded, this, &Cryptominder::updateWalletData);
        connect(add_wallet.get(), &AddWallet::walletAdded, this, &Cryptominder::populateTransactionList);
        add_wallet->setModal(true);
        add_wallet->exec();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
