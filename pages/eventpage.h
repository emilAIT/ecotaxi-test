#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>
#include <QStandardItemModel>

#include "../managers/nm.h"
#include "../managers/operations.h"
#include "../managers/usersession.h"

#include "../classes/event.h"
#include "../classes/charge.h"

#include "../pages/calendarpage.h"
#include "../pages/addupdatewindowevents.h"
#include "../pages/customdialog.h"

#include "../enums/Events.h"

namespace Ui
{
    class eventPage;
}

class eventPage : public QWidget
{
    Q_OBJECT

public:
    explicit eventPage(nm *nav, QWidget *parent = nullptr);
    ~eventPage();
    void setEvents(Events events, QDate date = QDate());

private slots:
    void on_BackButton_clicked();
    void on_EditButton_clicked();
    void on_DeleteButton_clicked();

    void on_DateButton_clicked();

    void on_ReportButton_clicked();

    void onDateChange(QDate date);

    void onSectionResized(int logicalIndex, int oldSize, int newSize);

    void onYes();
    void onNo();

private:
    Ui::eventPage *ui;
    nm *nav;
    Events table;

    void setHeader();
    bool setTable();
    void setBottomTable();

    void adjustColumnWidths();

    const int tableWidth = 1130;
    const int minColumnWidth = 100;

    QDate date;
};

#endif // EVENTPAGE_H
