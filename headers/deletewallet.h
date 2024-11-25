#ifndef DELETEWALLET_H
#define DELETEWALLET_H

#include <QDialog>
#include "basedb.h"

namespace Ui {
class DeleteWallet;
}

class DeleteWallet : public QDialog, public BaseDatabase
{
    Q_OBJECT

public:
    explicit DeleteWallet(QWidget *parent = nullptr);
    ~DeleteWallet() final;
    void receiveData(const QString &data);

private slots:
    void on_delete_wallet_button_clicked();

private:
    Ui::DeleteWallet *ui;

signals:
    void walletDeleted();

};

#endif // DELETEWALLET_H
