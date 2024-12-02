#ifndef ADDWALLET_H
#define ADDWALLET_H

#include <QDialog>
#include "basedb.h"
#include "DatabaseException.h"


namespace Ui {
class AddWallet;
}

class AddWallet : public QDialog, public BaseDatabase
{
    Q_OBJECT

public:
    explicit AddWallet(QWidget *parent = nullptr);
    ~AddWallet() final;

private slots:
    void on_pushButton_clicked();


private:
    Ui::AddWallet *ui;

signals:
    void wallet_added();

};

#endif // ADDWALLET_H
