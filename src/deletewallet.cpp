#include "deletewallet.h"
#include "ui/ui_deletewallet.h"

DeleteWallet::DeleteWallet(QWidget *parent)
    : QDialog(parent)
    , BaseDatabase()
    , ui(new Ui::DeleteWallet)
{
    ui->setupUi(this);
    createAccount("Mikalai", "test", "123");
    account->load_from_db(1);
    auto rows = account->get_wallets_from_db(1);
    for (const auto &row : rows) {
        QString wallet_address = QString::fromStdString(row["wallet_address"].c_str());
        ui->comboBox->addItem(wallet_address);
    }
}

DeleteWallet::~DeleteWallet() = default;

void DeleteWallet::on_delete_wallet_button_clicked()
{
    QString wallet_address_str = ui->comboBox->currentText();
    std::string wallet_address = wallet_address_str.toStdString();
    account->delete_wallet(wallet_address);
    emit walletDeleted();
    accept();
}

void DeleteWallet::receiveData(const QString &data) {
    ui->comboBox->setCurrentText(data);
}
