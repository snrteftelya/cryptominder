#include "send.h"
#include "ui/ui_send.h"
#include <QMessageBox>

Send::Send(QWidget *parent)
    : QDialog(parent)
    , BaseDatabase()
    , ui(new Ui::Send)
{
    ui->setupUi(this);
    create_account("Mikalai", "test", "123");
    account->load_from_db(1);
    auto rows = account->get_wallets_from_db(1);
    for (const auto &row : rows) {
        QString wallet_address = QString::fromStdString(row["wallet_address"].c_str());
        ui->comboBox->addItem(wallet_address);
    }
}

Send::~Send() = default;

void Send::on_send_wallet_button_clicked()
{
    try {
        if (!is_database_connected()) {
            throw DatabaseException("Database connection is broken.");
        }
        Wallet *wallet = nullptr;
        QString sender_address_str = ui->comboBox->currentText();
        std::string from_wallet_address = sender_address_str.toStdString();
        QString recipient_address_str = ui->recipient_address_input->text();
        std::string to_wallet_address = recipient_address_str.toStdString();
        QString wallet_balance_str = ui->wallet_balance_input->text();
        double wallet_balance = wallet_balance_str.toDouble();
        account->transfer_money(from_wallet_address, to_wallet_address, wallet_balance);
        emit send_money();
        accept();
    } catch (const DatabaseException &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void Send::receive_data(const QString &data) {
    ui->comboBox->setCurrentText(data);
}
