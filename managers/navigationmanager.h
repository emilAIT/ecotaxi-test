#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H

#include <QWidget>

#include "nm.h"

#include "../mainwindow.h"
#include "../pages/settingspage.h"
#include "../pages/eventpage.h"
#include "../pages/generalreport.h"
#include "../pages/reportpage.h"

#include "../enums/setting.h"
#include "../enums/Events.h"
#include "../enums/Report.h"

namespace Ui
{
    class navigationManager;
}

class navigationManager : public QWidget, public nm
{
    Q_OBJECT

    Q_INTERFACES(nm)

public:
    explicit navigationManager(QWidget *parent = nullptr);
    ~navigationManager();

    void changeWindow(int id);

    bool openSettings(int id);

    bool openEvents(int id, QDate = QDate());

    bool openReport(int index, int id = 0, QDate = QDate(), QDate = QDate());
private:
    Ui::navigationManager *ui;

    MainWindow *MainPage;

    SettingsPage *SettingPage;
    eventPage *EventPage;

    ReportPage *ReportsPage;
    GeneralReport *GReportPage;
};

#endif // NAVIGATIONMANAGER_H
