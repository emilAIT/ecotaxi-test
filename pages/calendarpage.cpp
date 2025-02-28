#include "calendarpage.h"
#include "ui_calendarpage.h"

CalendarPage::CalendarPage(QDate date, GeneralReport *report, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CalendarPage)
    , generalReport(report)  // Инициализируем указатель на GeneralReport
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    ui->calendarWidget->setSelectedDate(date);

    this->setWindowModality(Qt::ApplicationModal);
}

CalendarPage::~CalendarPage()
{
    delete ui;
}

void CalendarPage::on_confirmButton_clicked()
{
    emit changeDate(ui->calendarWidget->selectedDate());
    generalReport->setTable();        // Автоматически обновляем таблицу
    generalReport->setBottomTable();  // Автоматически обновляем нижнюю таблицу
    generalReport->setTableSizes();   // Обновляем размеры таблицы
    close();
}

void CalendarPage::on_cancelButton_clicked()
{
    this->close();
}
