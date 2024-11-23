#include "cryptominder.h"
#include "ui_cryptominder.h"
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
        qDebug() << "Selected text:" << ui->comboBox->itemText(index);
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

Cryptominder::~Cryptominder()
{
    delete ui;
    delete conn;
}

void Cryptominder::on_receive_button_clicked()
{
    Receive *receive_window = new Receive(this);
    connect(this, &Cryptominder::sendData, receive_window, &Receive::receiveData);
    QString walletAddress = ui->comboBox->currentText();
    emit sendData(walletAddress);
    receive_window->setModal(true);
    receive_window->exec();
}


void Cryptominder::on_send_button_clicked()
{
    QString label_text = ui->comboBox->currentText();
    qDebug() << "Selected text from comboBox:" << label_text;
    Send *send_window = new Send(this);
    connect(this, &Cryptominder::sendData, send_window, &Send::receiveData);
    emit sendData(label_text);
    connect(send_window, &Send::send_money, this, &Cryptominder::updateWalletData);
    connect(send_window, &Send::send_money, this, &Cryptominder::populateTransactionList);
    send_window->setModal(true);
    send_window->exec();
    delete send_window;
}


void Cryptominder::on_add_button_clicked()
{
    AddWallet *add_wallet = new AddWallet(this);
    connect(add_wallet, &AddWallet::walletAdded, this, &Cryptominder::updateWalletData);
    connect(add_wallet, &AddWallet::walletAdded, this, &Cryptominder::populateTransactionList);

    add_wallet->setModal(true);
    add_wallet->exec();
    delete add_wallet;
}

void Cryptominder::on_delete_wallet_clicked()
{
    DeleteWallet *delete_wallet = new DeleteWallet(this);
    QString label_text = ui->comboBox->currentText();
    qDebug() << "Selected text from comboBox:" << label_text;
    connect(this, &Cryptominder::sendData, delete_wallet, &DeleteWallet::receiveData);
    connect(delete_wallet, &DeleteWallet::walletDeleted, this, &Cryptominder::updateWalletData);
    connect(delete_wallet, &DeleteWallet::walletDeleted, this, &Cryptominder::populateTransactionList);
   emit sendData(label_text);
    populateTransactionList();
    delete_wallet->setModal(true);
    delete_wallet->exec();
    delete delete_wallet;
}


void Cryptominder::on_top_up_button_clicked()
{
    Wallet *wallet = nullptr;
    QString label_text = ui->comboBox->currentText();
    qDebug() << "Selected text from comboBox:" << label_text;
    std::string wallet_address = label_text.toStdString();
    if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
        *wallet + 50;
        double balance = wallet->get_wallet_balance();
        QString wallet_balance = QString::number(balance, 'f', 2);
        ui->money_label->setText(wallet_balance + " TON");
    }
    else {
        qDebug() << "Text from label: " << label_text;
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
    }
    else         ui->money_label->setText("? TON");
}


void Cryptominder::populateTransactionList() {
    ui->transactionTable->clear();
    QString label = ui->comboBox->currentText();
    std::string wallet_address = label.toStdString();
    conn = new pqxx::connection("dbname=crypto user=crypto_owner password=I6XNyohUfBj8 host=ep-yellow-truth-a2dw0siz.eu-central-1.aws.neon.tech sslmode=require");
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
    delete conn;
}




