#include "generalreport.h"
#include "pages/ui_generalreport.h"

GeneralReport::GeneralReport(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::GeneralReport)
{
    ui->setupUi(this);
    this->nav = nav;

    this->fromDate = QDate::currentDate().addMonths(-1);
    this->toDate = QDate::currentDate();

    ui->FromDateButton->setProperty("color", "gray");
    ui->ToDateButton->setProperty("color", "gray");

    ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
}

GeneralReport::~GeneralReport()
{
    delete ui;
}

void GeneralReport::setReport(Report mode, QDate from, QDate to)
{
    this->mode = mode;

    if (!from.isNull())
    {
        this->fromDate = from;
        this->toDate = to;

        ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
        ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
    }

    setHeader();

    setTable();
    setBottomTable();

    setTableSizes();
}

void GeneralReport::setHeader()
{
    switch (this->mode)
    {
    case Report::Cars:
        ui->Header->setText("ПО МАШИНАМ");
        ui->ReportButton->setText("ОТЧЕТ ПО МАШИНЕ");
        break;

    case Report::Drivers:
        ui->Header->setText("ПО ВОДИТЕЛЯМ");
        ui->ReportButton->setText("ОТЧЕТ ПО ВОДИТЕЛЮ");
        break;

    case Report::Investors:
        ui->Header->setText("ПО ИНВЕСТОРАМ");
        ui->ReportButton->setText("ОТЧЕТ ПО ИНВЕСТОРУ");
        break;

    case Report::Types:
        ui->Header->setText("ОБЩИЙ");
        ui->ReportButton->setText("ОТЧЕТ ПО ТИПУ");
        break;

    case Report::Locations:
        ui->Header->setText("ПО ЛОКАЦИЯМ");
        ui->ReportButton->setText("ОТЧЕТ ПО ЛОКАЦИИ");
        break;

    case Report::Charges:
        ui->Header->setText("ПО ЗАРЯДКАМ");
        ui->ReportButton->setText("ОТЧЕТ ПО ЗАРЯДКЕ");
        break;

    case Report::Users:
        ui->Header->setText("ПО ПОЛЬЗОВАТЕЛЯМ");
        ui->ReportButton->setText("ОТЧЕТ ПО ПОЛЬЗОВАТЕЛЮ");
        break;
    }
}

void GeneralReport::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->mode)
    {
    case Report::Cars:
        model->setHorizontalHeaderLabels({"uid", "ID", "Инвестор", "Доход", "Расход", "Прибыль", "Дней", "Средняя"});
        for (const QVariant &car : ReportOperations::getCarsReport(this->fromDate, this->toDate))
        {
            QVariantList cars = car.toList();
            QList<QStandardItem *> row;

            // Assume that the data in cars[] is in the correct order and types.
            row.append(new QStandardItem(cars[0].toString()));  // ids
            row.append(new QStandardItem(cars[1].toString()));  // ID
            row.append(new QStandardItem(cars[2].toString()));  // Инвестор

            // Ensure numerical data is set correctly for sorting
            QStandardItem *incomeItem = new QStandardItem();
            incomeItem->setData(cars[3].toInt(), Qt::DisplayRole);
            row.append(incomeItem); // Доход

            QStandardItem *expenseItem = new QStandardItem();
            expenseItem->setData(cars[4].toInt(), Qt::DisplayRole);
            row.append(expenseItem); // Расход

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(cars[5].toInt(), Qt::DisplayRole);
            row.append(profitItem); // Прибыль

            QStandardItem *daysItem = new QStandardItem();
            daysItem->setData(cars[6].toInt(), Qt::DisplayRole);
            row.append(daysItem); // Дней

            QStandardItem *averageItem = new QStandardItem();
            averageItem->setData(cars[7].toInt(), Qt::DisplayRole);
            row.append(averageItem); // Средняя

            model->appendRow(row);
        }

        break;
    case Report::Types:
        model->setHorizontalHeaderLabels({"ID", "Название", "Доход", "Расход", "Прибыль"});
        for (const QVariant &type : ReportOperations::getTypesReport(this->fromDate, this->toDate))
        {
            QVariantList types = type.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(types[0].toString()));  // ID
            row.append(new QStandardItem(types[1].toString()));  // Название

            // Ensure numerical data is set correctly for sorting
            QStandardItem *incomeItem = new QStandardItem();
            incomeItem->setData(types[2].toInt(), Qt::DisplayRole);
            row.append(incomeItem); // Доход

            QStandardItem *expenseItem = new QStandardItem();
            expenseItem->setData(types[3].toInt(), Qt::DisplayRole);
            row.append(expenseItem); // Расход

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(types[4].toInt(), Qt::DisplayRole);
            row.append(profitItem); // Прибыль

            model->appendRow(row);
        }

        break;
    case Report::Drivers:
        model->setHorizontalHeaderLabels({"ID", "Имя", "Доход", "Расход", "Прибыль"});
        for (const QVariant &driver : ReportOperations::getDriversReport(this->fromDate, this->toDate))
        {
            QVariantList drivers = driver.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(drivers[0].toString()));  // ID
            row.append(new QStandardItem(drivers[1].toString()));  // Имя

            // Ensure numerical data is set correctly for sorting
            QStandardItem *incomeItem = new QStandardItem();
            incomeItem->setData(drivers[2].toInt(), Qt::DisplayRole);
            row.append(incomeItem); // Доход

            QStandardItem *expenseItem = new QStandardItem();
            expenseItem->setData(drivers[3].toInt(), Qt::DisplayRole);
            row.append(expenseItem); // Расход

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(drivers[4].toInt(), Qt::DisplayRole);
            row.append(profitItem); // Прибыль

            model->appendRow(row);
        }
        break;
    case Report::Investors:
        model->setHorizontalHeaderLabels({"ID", "Имя", "Доход", "Расход", "Прибыль"});
        for (const QVariant &investor : ReportOperations::getInvestorsReport(this->fromDate, this->toDate))
        {
            QVariantList investors = investor.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(investors[0].toString()));  // ID
            row.append(new QStandardItem(investors[1].toString()));  // Имя

            // Ensure numerical data is set correctly for sorting as integers
            QStandardItem *incomeItem = new QStandardItem();
            incomeItem->setData(investors[2].toInt(), Qt::DisplayRole);
            row.append(incomeItem); // Доход

            QStandardItem *expenseItem = new QStandardItem();
            expenseItem->setData(investors[3].toInt(), Qt::DisplayRole);
            row.append(expenseItem); // Расход

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(investors[4].toInt(), Qt::DisplayRole);
            row.append(profitItem); // Прибыль

            model->appendRow(row);
        }
        break;
    case Report::Locations:
        model->setHorizontalHeaderLabels({"ID", "Название", "KWH"});
        for (const QVariant &location : ReportOperations::getLocationsReport(this->fromDate, this->toDate))
        {
            QVariantList locations = location.toList();
            QList<QStandardItem *> row;

            // Create QStandardItem for ID and Название as string
            row.append(new QStandardItem(locations[0].toString()));  // ID
            row.append(new QStandardItem(locations[1].toString()));  // Название

            // Ensure numerical data (KWH) is set correctly for sorting as float or double
            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(locations[2].toInt(), Qt::DisplayRole);  // Change to .toDouble() for float or double values
            row.append(kwhItem); // KWH

            model->appendRow(row);
        }
        break;
    case Report::Charges:
        model->setHorizontalHeaderLabels({"id", "ID Машины", "KWH", "Время"});
        for (const QVariant &charge : ReportOperations::getChargesReport(this->fromDate, this->toDate))
        {
            QVariantList charges = charge.toList();
            QList<QStandardItem *> row;

            // Create QStandardItem for ID Машины as string
            row.append(new QStandardItem(charges[0].toString()));  // id Машины
            row.append(new QStandardItem(charges[1].toString()));  // ID Машины

            // Ensure numerical data is set correctly for sorting as integers
            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(charges[2].toInt(), Qt::DisplayRole);  // KWH
            row.append(kwhItem);

            QStandardItem *timeItem = new QStandardItem();
            timeItem->setData(charges[3].toInt(), Qt::DisplayRole);  // Время
            row.append(timeItem);

            model->appendRow(row);
        }
        break;

    case Report::Users:
        model->setHorizontalHeaderLabels({"id", "ID", "Дата", "Имя"});
        for (const QVariant &user : ReportOperations::getUsersReport(this->fromDate, this->toDate))
        {
            QVariantList users = user.toList();
            QList<QStandardItem *> row;

            // Create QStandardItem for uid and ID as strings
            row.append(new QStandardItem(users[0].toString()));  // uid
            row.append(new QStandardItem(users[1].toString()));  // ID

            // Assuming "Дата" is a date or time, keep it as string if necessary
            row.append(new QStandardItem(users[2].toString()));  // Дата

            // Create QStandardItem for Имя as string
            row.append(new QStandardItem(users[3].toString()));  // Имя

            model->appendRow(row);
        }
        break;

    default:
        break;
    }

    ui->tableView->setModel(model);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &GeneralReport::onSectionResized);
}

void GeneralReport::setBottomTable()
{
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->mode)
    {
    case Report::Cars:
        for (const QVariant &car : ReportOperations::getAllCarsReport(this->fromDate, this->toDate))
        {
            QVariantList cars = car.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              "",
                                              cars[0].toString(),
                                              cars[1].toString(),
                                              cars[2].toString()});
        }
        break;

    case Report::Types:
        for (const QVariant &type : ReportOperations::getAllTypesReport(this->fromDate, this->toDate))
        {
            QVariantList types = type.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              "",
                                              types[0].toString(),
                                              types[1].toString(),
                                              types[2].toString()});
        }
        break;

    case Report::Drivers:
        for (const QVariant &driver : ReportOperations::getAllDriversReport(this->fromDate, this->toDate))
        {
            QVariantList drivers = driver.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              "",
                                              drivers[0].toString(),
                                              drivers[1].toString(),
                                              drivers[2].toString()});
        }
        break;

    case Report::Investors:
        for (const QVariant &investor : ReportOperations::getAllInvestorsReport(this->fromDate, this->toDate))
        {
            QVariantList investors = investor.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              "",
                                              investors[0].toString(),
                                              investors[1].toString(),
                                              investors[2].toString()});
        }
        break;

    case Report::Locations:
        for (const QVariant &location : ReportOperations::getAllLocationsReport(this->fromDate, this->toDate))
        {
            QVariantList locations = location.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                locations[0].toString(),
            });
        }
        break;

    case Report::Charges:
        for (const QVariant &charge : ReportOperations::getAllChargesReport(this->fromDate, this->toDate))
        {
            QVariantList charges = charge.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                charges[0].toString(),
            });
        }
        break;

    case Report::Users:
        for (const QVariant &user : ReportOperations::getAllUsersReport(this->fromDate, this->toDate))
        {
            QVariantList users = user.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                users[0].toString(),
            });
        }
        break;

    default:
        break;
    }

    ui->bottomTable->setModel(model);

    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void GeneralReport::setTableSizes()
{
    switch (this->mode)
    {
    case Report::Cars:
        ui->tableView->setColumnWidth(0, 161);
        ui->tableView->setColumnWidth(1, 161);
        ui->tableView->setColumnWidth(2, 161);
        ui->tableView->setColumnWidth(3, 161);
        ui->tableView->setColumnWidth(4, 161);
        ui->tableView->setColumnWidth(5, 161);
        ui->tableView->setColumnWidth(6, 161);
        break;

    case Report::Types:
        ui->tableView->setColumnWidth(0, 226);
        ui->tableView->setColumnWidth(1, 226);
        ui->tableView->setColumnWidth(2, 226);
        ui->tableView->setColumnWidth(3, 226);
        ui->tableView->setColumnWidth(4, 226);
        break;

    case Report::Drivers:
        ui->tableView->setColumnWidth(0, 226);
        ui->tableView->setColumnWidth(1, 226);
        ui->tableView->setColumnWidth(2, 226);
        ui->tableView->setColumnWidth(3, 226);
        ui->tableView->setColumnWidth(4, 226);
        break;

    case Report::Investors:
        ui->tableView->setColumnWidth(0, 226);
        ui->tableView->setColumnWidth(1, 226);
        ui->tableView->setColumnWidth(2, 226);
        ui->tableView->setColumnWidth(3, 226);
        ui->tableView->setColumnWidth(4, 226);
        break;

    case Report::Locations:
        ui->tableView->setColumnWidth(0, 377);
        ui->tableView->setColumnWidth(1, 377);
        ui->tableView->setColumnWidth(2, 377);
        break;

    case Report::Charges:
        ui->tableView->setColumnWidth(0, 377);
        ui->tableView->setColumnWidth(1, 377);
        ui->tableView->setColumnWidth(2, 377);
        break;

    case Report::Users:
        ui->tableView->setColumnWidth(0, 377);
        ui->tableView->setColumnWidth(1, 377);
        ui->tableView->setColumnWidth(2, 377);
        break;

    default:
        break;
    }
}

void GeneralReport::on_SettingsButton_clicked()
{
    switch (this->mode)
    {
    case Report::Drivers:
        nav->openSettings(0);
        break;
    case Report::Cars:
        nav->openSettings(1);
        break;
    case Report::Investors:
        nav->openSettings(2);
        break;
    case Report::Types:
        nav->openSettings(3);
        break;
    case Report::Locations:
        nav->openSettings(4);
        break;
    case Report::Charges:
        nav->openSettings(6);
        break;
    case Report::Users:
        nav->openSettings(5);
        break;
    }
}

void GeneralReport::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void GeneralReport::on_ReportButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->mode)
        {
        case Report::Cars:
            nav->openReport(9, id, fromDate, toDate);
            break;
        case Report::Types:
            nav->openReport(7, id, fromDate, toDate);
            break;
        case Report::Drivers:
            nav->openReport(10, id, fromDate, toDate);
            break;
        case Report::Investors:
            nav->openReport(8, id, fromDate, toDate);
            break;
        case Report::Locations:
            nav->openReport(12, id, fromDate, toDate);
            break;
        case Report::Charges:
            nav->openReport(13, id, fromDate, toDate);
            break;
        case Report::Users:
            nav->openReport(11, id, fromDate, toDate);
            break;
        }
    }
    else
    {
        switch (this->mode)
        {
        case Report::Cars:
            nav->openReport(9, 0, fromDate, toDate);
            break;
        case Report::Types:
            nav->openReport(7, 0, fromDate, toDate);
            break;
        case Report::Drivers:
            nav->openReport(10, 0, fromDate, toDate);
            break;
        case Report::Investors:
            nav->openReport(8, 0, fromDate, toDate);
            break;
        case Report::Locations:
            nav->openReport(12, 0, fromDate, toDate);
            break;
        case Report::Charges:
            nav->openReport(13, 0, fromDate, toDate);
            break;
        case Report::Users:
            nav->openReport(11, 0, fromDate, toDate);
            break;
        }
    }
}

void GeneralReport::on_FromDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->fromDate);

    connect(c, &CalendarPage::changeDate, this, &GeneralReport::setFromDate);

    c->show();
}

void GeneralReport::on_ToDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->toDate);

    connect(c, &CalendarPage::changeDate, this, &GeneralReport::setToDate);

    c->show();
}

void GeneralReport::setFromDate(QDate date)
{
    this->fromDate = date;
    ui->FromDateButton->setText(date.toString("dd.MM.yyyy"));
}

void GeneralReport::setToDate(QDate date)
{
    this->toDate = date;
    ui->ToDateButton->setText(date.toString("dd.MM.yyyy"));
}

void GeneralReport::on_FilterButton_clicked()
{
    setTable();
    setBottomTable();

    setTableSizes();
}

void GeneralReport::on_ToPDFButton_clicked()
{
    switch (this->mode)
    {
    case Report::Cars:
        PDFmanager::createCarsReport(this->fromDate, this->toDate);
        break;

    case Report::Drivers:
        PDFmanager::createDriversReport(this->fromDate, this->toDate);
        break;

    case Report::Investors:
        PDFmanager::createInvestorsReport(this->fromDate, this->toDate);
        break;

    case Report::Types:
        PDFmanager::createTypesReport(this->fromDate, this->toDate);
        break;

    case Report::Locations:
        PDFmanager::createLocationsReport(this->fromDate, this->toDate);
        break;

    case Report::Charges:
        PDFmanager::createChargesReport(this->fromDate, this->toDate);
        break;

    case Report::Users:
        PDFmanager::createUsersReport(this->fromDate, this->toDate);
        break;
    }
}

void GeneralReport::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}

void GeneralReport::adjustColumnWidths()
{
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i)
    {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 1130 - totalWidth;
    if (lastColumnWidth > 150)
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    }
    else
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}
