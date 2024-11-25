#ifndef RECEIVE_H
#define RECEIVE_H

#include <QDialog>
#include "basedb.h"

namespace Ui {
class Receive;
}

class Receive : public QDialog, public BaseDatabase
{
    Q_OBJECT

public:
    explicit Receive(QWidget *parent = nullptr);
    ~Receive();
    void receiveData(const QString &data);

private slots:
    void on_copyButton_clicked();

private:
    Ui::Receive *ui;
};

#endif // RECEIVE_H
