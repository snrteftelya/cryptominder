#include "addwallet.h"
#include "ui/ui_addwallet.h"

AddWallet::AddWallet(QWidget *parent)
    : QDialog(parent)
    , BaseDatabase()
    , ui(new Ui::AddWallet)
{
    ui->setupUi(this);
    create_account("Mikalai", "test", "123");
    account->load_from_db(1);
}

AddWallet::~AddWallet() = default;

void AddWallet::on_pushButton_clicked()
{
    QString wallet_balance_str = ui->wallet_balance_input->text();
    double wallet_balance = wallet_balance_str.toDouble();
    account->add_wallet(wallet_balance);
    emit wallet_added();
    accept();
}
