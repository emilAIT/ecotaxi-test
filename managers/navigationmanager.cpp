#include "navigationmanager.h"
#include "ui_navigationmanager.h"

navigationManager::navigationManager(QWidget *parent)
    : QWidget(parent), nm(), ui(new Ui::navigationManager)
{
    ui->setupUi(this);

    this->MainPage = new MainWindow(this);

    this->SettingPage = new SettingsPage(this);
    this->EventPage = new eventPage(this);

    this->ReportsPage = new ReportPage(this);
    this->GReportPage = new GeneralReport(this);

    ui->pages->insertWidget(0, this->MainPage);

    ui->pages->insertWidget(1, this->SettingPage);
    ui->pages->insertWidget(2, this->EventPage);

    ui->pages->insertWidget(3, this->GReportPage);
    ui->pages->insertWidget(4, this->ReportsPage);

    this->changeWindow(0);
}

navigationManager::~navigationManager()
{
    delete ui;
}

void navigationManager::changeWindow(int id)
{
    if (id == 0)
    {
        this->MainPage->openWidnow();
    }
    ui->pages->setCurrentIndex(id);
}

bool navigationManager::openSettings(int id)
{
    switch (id)
    {
    case 0:
        this->SettingPage->setSettings(Setting::Drivers);
        break;
    case 1:
        this->SettingPage->setSettings(Setting::Cars);
        break;
    case 2:
        this->SettingPage->setSettings(Setting::Investors);
        break;
    case 3:
        this->SettingPage->setSettings(Setting::Types);
        break;
    case 4:
        this->SettingPage->setSettings(Setting::Locations);
        break;
    case 5:
        this->SettingPage->setSettings(Setting::Users);
        break;
    default:
        return false;
    }
    changeWindow(1);
    return true;
}

bool navigationManager::openEvents(int id, QDate date)
{
    switch (id)
    {
    case 0:
        this->EventPage->setEvents(Events::Events, date);
        break;
    case 1:
        this->EventPage->setEvents(Events::Charges, date);
        break;
    default:
        return false;
    }
    changeWindow(2);
    return true;
}

bool navigationManager::openReport(int index, int id, QDate from, QDate to)
{
    switch (index)
    {
    case 0:
        this->GReportPage->setReport(Report::Investors, from, to);
        changeWindow(3);
        break;
    case 1:
        this->GReportPage->setReport(Report::Types, from, to);
        changeWindow(3);
        break;
    case 2:
        this->GReportPage->setReport(Report::Cars, from, to);
        changeWindow(3);
        break;
    case 3:
        this->GReportPage->setReport(Report::Drivers, from, to);
        changeWindow(3);
        break;
    case 4:
        this->GReportPage->setReport(Report::Locations, from, to);
        changeWindow(3);
        break;
    case 5:
        this->GReportPage->setReport(Report::Users, from, to);
        changeWindow(3);
        break;
    case 6:
        this->GReportPage->setReport(Report::Charges, from, to);
        changeWindow(3);
        break;
    case 7:
        this->ReportsPage->setReport(Report::Types, id, from, to);
        changeWindow(4);
        break;
    case 8:
        this->ReportsPage->setReport(Report::Investors, id, from, to);
        changeWindow(4);
        break;
    case 9:
        this->ReportsPage->setReport(Report::Cars, id, from, to);
        changeWindow(4);
        break;
    case 10:
        this->ReportsPage->setReport(Report::Drivers, id, from, to);
        changeWindow(4);
        break;
    case 11:
        this->ReportsPage->setReport(Report::Users, id, from, to);
        changeWindow(4);
        break;
    case 12:
        this->ReportsPage->setReport(Report::Locations, id, from, to);
        changeWindow(4);
        break;
    case 13:
        this->ReportsPage->setReport(Report::Charges, id, from, to);
        changeWindow(4);
        break;
    default:
        return false;
        break;
    }
    return true;
}
