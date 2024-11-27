#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>
#include "basedb.h"

namespace Ui {
class Statistics;
}

class Statistics : public QDialog, public BaseDatabase
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = nullptr);
    ~Statistics() final;
    void receiveData(const QString &data);

private slots:
    void on_get_statistics_button_clicked();

    void on_calendar_clicked(const QDate &date);

    void on_is_sender_clicked();

    void on_is_reciever_clicked();

private:
    Ui::Statistics *ui;
    QString selected_date;
    bool is_sender_checked = false;
    bool is_receiver_checked = false;
    QString received_data;
};

#endif // STATISTICS_H
