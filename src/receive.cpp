#include "receive.h"
#include "ui/ui_receive.h"
#include <QClipboard>
#include <QApplication>

Receive::Receive(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Receive)
{
    ui->setupUi(this);
    connect(ui->copyButton, &QPushButton::clicked, this, &Receive::on_copyButton_clicked);
}

Receive::~Receive() = default;

void Receive::on_copyButton_clicked()
{
    QString walletAddress = ui->walletAddressLabel->text();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(walletAddress);
    accept();
}

void Receive::receive_data(const QString &data) {
    ui->walletAddressLabel->setText(data);
}
