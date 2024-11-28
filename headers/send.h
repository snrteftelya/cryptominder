#ifndef SEND_H
#define SEND_H

#include <QDialog>
#include "basedb.h"

namespace Ui {
class Send;
}

class Send : public QDialog, public BaseDatabase
{
    Q_OBJECT

public:
    explicit Send(QWidget *parent = nullptr);
    ~Send() final;
    void receive_data(const QString &data);

private slots:
    void on_send_wallet_button_clicked();

private:
    Ui::Send *ui;

signals:
    void send_money();

};

#endif // SEND_H
