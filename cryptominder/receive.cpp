#include "receive.h"
#include "ui_receive.h"
#include <QClipboard>
#include <QApplication>

Receive::Receive(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Receive)
{
    ui->setupUi(this);
    connect(ui->copyButton, &QPushButton::clicked, this, &Receive::on_copyButton_clicked);
}

Receive::~Receive()
{
    delete ui;
}

void Receive::on_copyButton_clicked()
{
    QString walletAddress = ui->walletAddressLabel->text();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(walletAddress);
    accept();
}

void Receive::receiveData(const QString &data) {
    ui->walletAddressLabel->setText(data);
}
