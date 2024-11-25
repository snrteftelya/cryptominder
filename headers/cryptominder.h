#ifndef CRYPTOMINDER_H
#define CRYPTOMINDER_H

#include <QMainWindow>
#include "basedb.h"

namespace Ui {
class Cryptominder;
}

class Cryptominder : public QMainWindow, public BaseDatabase
{
    Q_OBJECT

public:
    explicit Cryptominder(QWidget *parent = nullptr);
    ~Cryptominder() final;

private slots:
    void on_receive_button_clicked();

    void on_send_button_clicked();

    void on_add_button_clicked();

    void on_delete_wallet_clicked();

    void on_top_up_button_clicked();

    void populateTransactionList();

    void updateWalletData();

private:
    Ui::Cryptominder *ui;

signals:
    void sendData(const QString &data);
};

#endif // CRYPTOMINDER_H
