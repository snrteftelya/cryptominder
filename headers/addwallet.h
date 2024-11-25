#ifndef ADDWALLET_H
#define ADDWALLET_H

#include <QDialog>
#include "basedb.h"

namespace Ui {
class AddWallet;
}

class AddWallet : public QDialog, public BaseDatabase
{
    Q_OBJECT

public:
    explicit AddWallet(QWidget *parent = nullptr);
    ~AddWallet();

private slots:
    void on_pushButton_clicked();


private:
    Ui::AddWallet *ui;

signals:
    void walletAdded();

};

#endif // ADDWALLET_H
