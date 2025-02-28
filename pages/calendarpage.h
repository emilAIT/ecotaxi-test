#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include <QWidget>
#include <QDate>
#include "generalreport.h"  // Добавляем include для GeneralReport

namespace Ui {
class CalendarPage;
}

class CalendarPage : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarPage(QDate date, GeneralReport *report, QWidget *parent = nullptr);  // Передаем указатель на GeneralReport
    ~CalendarPage();

signals:
    void changeDate(QDate date);

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::CalendarPage *ui;
    GeneralReport *generalReport;  // Указатель на GeneralReport
};

#endif // CALENDARPAGE_H
