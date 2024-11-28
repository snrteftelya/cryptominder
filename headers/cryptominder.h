#ifndef CRYPTOMINDER_H
#define CRYPTOMINDER_H

#include <QMainWindow>
#include "basedb.h"
#include "DatabaseException.h"

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

    void on_add_wallet_clicked();

    void on_delete_wallet_clicked();

    void on_top_up_button_clicked();

    void populate_transaction_list();

    void update_wallet_data();

    void on_get_statistics_clicked();

    bool is_database_connected();

private:
    Ui::Cryptominder *ui;

signals:
    void send_data(const QString &data);
};

#endif // CRYPTOMINDER_H
