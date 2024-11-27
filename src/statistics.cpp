#include "statistics.h"
#include "ui/ui_statistics.h"
#include <QCalendarWidget>
#include <QDebug>
#include "Transaction.h"
#include <QFile>

Statistics::Statistics(QWidget *parent)
    : QDialog(parent)
    , BaseDatabase()
    , ui(new Ui::Statistics)
{
    ui->setupUi(this);
    createAccount("Mikalai", "test", "123");
    account->load_from_db(1);
    auto rows = account->get_wallets_from_db(1);

    connect(ui->calendar, &QCalendarWidget::clicked, this, &Statistics::on_calendar_clicked);
    connect(ui->is_sender, &QRadioButton::clicked, this, &Statistics::on_is_sender_clicked);
    connect(ui->is_reciever, &QRadioButton::clicked, this, &Statistics::on_is_reciever_clicked);
}

Statistics::~Statistics() = default;

void Statistics::on_calendar_clicked(const QDate &date)
{
    selected_date = date.toString("yyyy-MM-dd");
}

void Statistics::on_is_sender_clicked()
{
    is_sender_checked = ui->is_sender->isChecked();
}

void Statistics::on_is_reciever_clicked()
{
    is_receiver_checked = ui->is_reciever->isChecked();
}

void Statistics::on_get_statistics_button_clicked()
{
    std::string sender_wallet_address;
    std::string receiver_wallet_address;
    std::string created_at;

    if (is_receiver_checked) receiver_wallet_address = received_data.toStdString();
    if (is_sender_checked) sender_wallet_address = received_data.toStdString();
    created_at = selected_date.toStdString();
    if (!is_receiver_checked && !is_sender_checked) {
        sender_wallet_address = received_data.toStdString();
        receiver_wallet_address = received_data.toStdString();
    }

    qDebug() << "Getting statistics...";
    qDebug() << "Selected date:" << selected_date;
    qDebug() << "Is sender checked:" << is_sender_checked;
    qDebug() << "Is receiver checked:" << is_receiver_checked;
    qDebug() << "Receiver wallet address:" << QString::fromStdString(receiver_wallet_address);
    qDebug() << "Sender wallet address:" << QString::fromStdString(sender_wallet_address);

    BaseDatabase database;
    pqxx::connection *conn = database.getConnection();
    Transaction transaction(*conn);

    try {
        nlohmann::json report = transaction.get_filtered_report(sender_wallet_address,
                                                                receiver_wallet_address,
                                                                created_at);

        QFile file("/Users/snrteftelya/Desktop/report.json");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing.";
            return;
        }

        QTextStream out(&file);
        out << QString::fromStdString(report.dump(4));
        file.close();

        qDebug() << "Report written successfully!";
    } catch (const pqxx::sql_error &e) {
        qDebug() << "SQL error:" << e.what();
    } catch (const std::exception &e) {
        qDebug() << "An error occurred:" << e.what();
    }
    accept();
}



void Statistics::receiveData(const QString &data) {
    received_data = data;
}
