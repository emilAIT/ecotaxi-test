#include "reportpage.h"
#include "ui_reportpage.h"

ReportPage::ReportPage(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::ReportPage)
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

ReportPage::~ReportPage()
{
    delete ui;
}

void ReportPage::setReport(Report mode, int id, QDate from, QDate to)
{
    this->mode = mode;

    if (!from.isNull())
    {
        this->fromDate = from;
        this->toDate = to;

        ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
        ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
    }

    this->id = id;

    setHeader();
    setSideTable();

    if (id != 0)
    {
        setTable();
        setBottomTable();
        setTableSizes();
    }
    else
    {
        QStandardItemModel *model = new QStandardItemModel();
        ui->tableView->setModel(model);
    }
}

void ReportPage::setHeader()
{
    switch (this->mode)
    {
    case Report::Cars:
        ui->Header->setText("ПО МАШИНЕ");
        ui->ReportButton->setText("ОТЧЕТ ПО МАШИНАМ");
        break;

    case Report::Drivers:
        ui->Header->setText("ПО ВОДИТЕЛЮ");
        ui->ReportButton->setText("ОТЧЕТ ПО ВОДИТЕЛЯМ");
        break;

    case Report::Investors:
        ui->Header->setText("ПО ИНВЕСТОРУ");
        ui->ReportButton->setText("ОТЧЕТ ПО ИНВЕСТОРАМ");
        break;

    case Report::Types:
        ui->Header->setText("ПО ТИПУ");
        ui->ReportButton->setText("ОТЧЕТ ПО ТИПАМ");
        break;

    case Report::Locations:
        ui->Header->setText("ПО ЛОКАЦИИ");
        ui->ReportButton->setText("ОТЧЕТ ПО ЛОКАЦИЯМ");
        break;

    case Report::Charges:
        ui->Header->setText("ПО ЗАРЯДКЕ");
        ui->ReportButton->setText("ОТЧЕТ ПО ЗАРЯДКАМ");
        break;

    case Report::Users:
        ui->Header->setText("ПО ПОЛЬЗОВАТЕЛЮ");
        ui->ReportButton->setText("ОТЧЕТ ПО ПОЛЬЗОВАТЕЛЯМ");
        break;
    }
}

void ReportPage::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    switch (this->mode)
    {
    case Report::Cars:
        model->setHorizontalHeaderLabels({"Дата", "Тип", "Водитель", "Сумма", "Описание"});
        for (const QVariant &carData : ReportOperations::getCarReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList car = carData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(car[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Other columns as string
            row.append(new QStandardItem(car[1].toString())); // Тип
            row.append(new QStandardItem(car[2].toString())); // Водитель

            // Ensure numerical data (Сумма) is set correctly for sorting as integers
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(car[3].toInt(), Qt::DisplayRole); // Сумма
            row.append(amountItem);

            row.append(new QStandardItem(car[4].toString())); // Описание

            model->appendRow(row);
        }
        break;
    case Report::Drivers:
        model->setHorizontalHeaderLabels({"Дата", "Тип", "Машина", "Сумма", "Описание"});
        for (const QVariant &driverData : ReportOperations::getDriverReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList driver = driverData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(driver[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Тип and Машина as strings
            row.append(new QStandardItem(driver[1].toString())); // Тип
            row.append(new QStandardItem(driver[2].toString())); // Машина

            // Ensure numerical data (Сумма) is set correctly for sorting as integers
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(driver[3].toInt(), Qt::DisplayRole); // Сумма
            row.append(amountItem);

            // Create QStandardItem for Описание as string
            row.append(new QStandardItem(driver[4].toString())); // Описание

            model->appendRow(row);
        }
        break;
    case Report::Investors:
        model->setHorizontalHeaderLabels({"id", "Машина", "Доход", "Расход", "Прибыль"});
        for (const QVariant &investorData : ReportOperations::getInvestorReport(this->id, this->fromDate, this->toDate))
        {
            qDebug() << "1";
            QVariantList investor = investorData.toList();
            qDebug() << "2";
            model->appendRow({new QStandardItem(investor[0].toString()),
                              new QStandardItem(investor[1].toString()),
                              new QStandardItem(investor[2].toString()),
                              new QStandardItem(investor[3].toString()),
                              new QStandardItem(investor[4].toString())});
        }
        break;
    case Report::Types:
        model->setHorizontalHeaderLabels({"Дата", "Водитель", "Машина", "Сумма", "Описание"});
        for (const QVariant &typeData : ReportOperations::getTypeReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList type = typeData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(type[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Водитель, Машина, and Описание as strings
            row.append(new QStandardItem(type[1].toString())); // Водитель
            row.append(new QStandardItem(type[2].toString())); // Машина

            // Ensure numerical data (Сумма) is set correctly for sorting as integers
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(type[3].toInt(), Qt::DisplayRole); // Сумма
            row.append(amountItem);

            // Create QStandardItem for Описание as string
            row.append(new QStandardItem(type[4].toString())); // Описание

            model->appendRow(row);
        }
        break;

    case Report::Locations:
        model->setHorizontalHeaderLabels({"Дата", "Машина", "Водитель", "KWH", "Время"});
        for (const QVariant &locationData : ReportOperations::getLocationReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList location = locationData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(location[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Машина and Водитель as strings
            row.append(new QStandardItem(location[1].toString())); // Машина
            row.append(new QStandardItem(location[2].toString())); // Водитель

            // Ensure numerical data (KWH and Время) is set correctly for sorting as integers
            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(location[3].toInt(), Qt::DisplayRole); // KWH
            row.append(kwhItem);

            QStandardItem *timeItem = new QStandardItem();
            timeItem->setData(location[4].toInt(), Qt::DisplayRole); // Время
            row.append(timeItem);

            model->appendRow(row);
        }
        break;

    case Report::Charges:
        model->setHorizontalHeaderLabels({"Дата", "Водитель", "Локация", "KWH", "Время"});
        for (const QVariant &chargeData : ReportOperations::getChargesByCarReport(this->id, this->fromDate, this->toDate))
        {
            qDebug() << chargeData;
            QVariantList charge = chargeData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(charge[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Водитель and Локация as strings
            row.append(new QStandardItem(charge[1].toString())); // Водитель
            row.append(new QStandardItem(charge[2].toString())); // Локация

            // Ensure numerical data (KWH and Время) is set correctly for sorting as integers
            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(charge[3].toInt(), Qt::DisplayRole); // KWH
            row.append(kwhItem);

            QStandardItem *timeItem = new QStandardItem();
            timeItem->setData(charge[4].toInt(), Qt::DisplayRole); // Время
            row.append(timeItem);

            model->appendRow(row);
        }

        break;

    case Report::Users:
        model->setHorizontalHeaderLabels({"ID", "Дата"});
        for (const QVariant &userData : ReportOperations::getUserReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList user = userData.toList();
            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(user[1].toDateTime(), Qt::DisplayRole);
            model->appendRow({new QStandardItem(user[0].toString()), dateItem});
        }
        break;
    }

    ui->tableView->setModel(model);

    if (this->mode == Report::Investors)
    {
        ui->tableView->setColumnHidden(0, true);
        ui->ItemButton->setDisabled(false);
    }
    else
    {
        ui->tableView->setColumnHidden(0, false);
        ui->ItemButton->setDisabled(true);
    }
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &ReportPage::onSectionResized);
}

void ReportPage::setBottomTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    switch (this->mode)
    {
    case Report::Cars:
        if (true)
        {
            QVariantList report = ReportOperations::getAllCarReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "",
                report[0].toString(),
                report[1].toString(),
                report[2].toString(),
            });
        }
        break;
    case Report::Drivers:
        if (true)
        {
            QVariantList report = ReportOperations::getAllDriverReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "",
                report[0].toString(),
                report[1].toString(),
                report[2].toString(),
            });
        }
        break;
    case Report::Investors:
        if (true)
        {
            QVariantList report = ReportOperations::getAllInvestorReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                report[1].toString(),
                report[2].toString(),
                report[3].toString(),
            });
        }
        break;
    case Report::Types:
        if (true)
        {
            QVariantList report = ReportOperations::getAllTypeReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "",
                report[0].toString(),
                report[1].toString(),
                report[2].toString(),
            });
        }
        break;
    case Report::Locations:
        if (true)
        {
            QVariantList report = ReportOperations::getAllLocationReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                report[0].toString(),
            });
        }
        break;
    case Report::Charges:
        if (true)
        {
            QVariantList report = ReportOperations::getAllChargesByCarReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                report[0].toString(),
            });
        }
        break;

    case Report::Users:
        if (true)
        {
            QVariantList report = ReportOperations::getAllUserReport(this->id, this->fromDate, this->toDate);
            qDebug() << report;
            model->setHorizontalHeaderLabels({
                "Итого",
                report[0].toList()[0].toString(),
            });
        }
        break;
    }

    ui->bottomTable->setModel(model);

    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ReportPage::setSideTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    int row = 0;
    switch (this->mode)
    {
    case Report::Cars:
        model->setHorizontalHeaderLabels({"id", "Машины"});
        for (Car car : Operations::selectAllCars())
        {
            if (this->id != 0 && car.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(car.getId())), new QStandardItem(QString::number(car.getSid()))});
        }
        break;

    case Report::Drivers:
        model->setHorizontalHeaderLabels({"id", "Водители"});
        for (Driver driver : Operations::selectAllDrivers())
        {
            if (this->id != 0 && driver.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(driver.getId())), new QStandardItem(driver.getName())});
        }
        break;

    case Report::Investors:
        model->setHorizontalHeaderLabels({"id", "Инвесторы"});
        for (Investor investor : Operations::selectAllInvestors())
        {
            if (this->id != 0 && investor.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(investor.getId())), new QStandardItem(investor.getName())});
        }
        break;

    case Report::Types:
        model->setHorizontalHeaderLabels({"id", "Типы"});
        for (Type type : Operations::selectAllTypes())
        {
            if (this->id != 0 && type.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(type.getId())), new QStandardItem(type.getName())});
        }
        break;

    case Report::Locations:
        model->setHorizontalHeaderLabels({"id", "Локации"});
        for (Location location : Operations::selectAllLocations())
        {
            if (this->id != 0 && location.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(location.getId())), new QStandardItem(location.getName())});
        }
        break;

    case Report::Charges:
        model->setHorizontalHeaderLabels({"id", "Машины"});
        for (Car car : Operations::selectAllCars())
        {
            if (this->id != 0 && car.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(car.getId())), new QStandardItem(QString::number(car.getId()))});
        }
        break;

    case Report::Users:
        model->setHorizontalHeaderLabels({"id", "Пользователи"});
        for (User user : Operations::selectAllUsers())
        {
            if (this->id != 0 && user.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(user.getId())), new QStandardItem(user.getName())});
        }
        break;
    }

    ui->sideTable->setModel(model);

    if (this->id != 0)
        ui->sideTable->selectRow(row);

    ui->sideTable->setColumnHidden(0, true);

    ui->sideTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->sideTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ReportPage::selectionChanged);
}

void ReportPage::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    int row = selected.indexes()[0].row();

    this->id = ui->sideTable->model()->index(row, 0).data().toLongLong();

    setTable();

    setBottomTable();

    setTableSizes();
}

void ReportPage::setTableSizes()
{
    switch (this->mode)
    {

    case Report::Cars:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Drivers:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Investors:
        ui->tableView->setColumnWidth(0, 215);
        ui->tableView->setColumnWidth(1, 215);
        ui->tableView->setColumnWidth(2, 215);
        ui->tableView->setColumnWidth(3, 215);
        break;

    case Report::Types:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Locations:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Charges:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Users:
        ui->tableView->setColumnWidth(0, 344);
        ui->tableView->setColumnWidth(1, 344);
    }
}

void ReportPage::on_SettingsButton_clicked()
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
        nav->openEvents(1);
        break;
    case Report::Users:
        nav->openSettings(5);
        break;
    }
}

void ReportPage::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void ReportPage::on_ReportButton_clicked()
{
    switch (this->mode)
    {
    case Report::Cars:
        nav->openReport(2, 0, fromDate, toDate);
        break;
    case Report::Types:
        nav->openReport(1, 0, fromDate, toDate);
        break;
    case Report::Drivers:
        nav->openReport(3, 0, fromDate, toDate);
        break;
    case Report::Investors:
        nav->openReport(0, 0, fromDate, toDate);
        break;
    case Report::Locations:
        nav->openReport(4, 0, fromDate, toDate);
        break;
    case Report::Charges:
        nav->openReport(6, 0, fromDate, toDate);
        break;
    case Report::Users:
        nav->openReport(5, 0, fromDate, toDate);
        break;
    }
}

void ReportPage::on_FromDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->fromDate);

    connect(c, &CalendarPage::changeDate, this, &ReportPage::setFromDate);

    c->show();
}

void ReportPage::on_ToDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->toDate);

    connect(c, &CalendarPage::changeDate, this, &ReportPage::setToDate);

    c->show();
}

void ReportPage::on_ToPDFButton_clicked()
{
    switch (this->mode)
    {
    case Report::Cars:
        PDFmanager::createCarReport(this->fromDate, this->toDate, this->id);
        break;

    case Report::Drivers:
        PDFmanager::createDriverReport(this->fromDate, this->toDate, this->id);
        break;

    case Report::Investors:
        PDFmanager::createInvestorReport(this->fromDate, this->toDate, this->id);
        break;

    case Report::Types:
        PDFmanager::createTypeReport(this->fromDate, this->toDate, this->id);
        break;

    case Report::Locations:
        PDFmanager::createLocationReport(this->fromDate, this->toDate, this->id);
        break;

    case Report::Charges:
        PDFmanager::createChargeReport(this->fromDate, this->toDate, this->id);
        break;

    case Report::Users:
        PDFmanager::createUserReport(this->fromDate, this->toDate, this->id);
        break;
    }
}

void ReportPage::setFromDate(QDate date)
{
    this->fromDate = date;
    ui->FromDateButton->setText(date.toString("dd.MM.yyyy"));
}

void ReportPage::setToDate(QDate date)
{
    this->toDate = date;
    ui->ToDateButton->setText(date.toString("dd.MM.yyyy"));
}

void ReportPage::on_FilterButton_clicked()
{
    setTable();
    setBottomTable();

    setTableSizes();
}

void ReportPage::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}

void ReportPage::adjustColumnWidths()
{
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i)
    {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 860 - totalWidth;
    if (lastColumnWidth > 150)
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    }
    else
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}

void ReportPage::on_ItemButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->mode)
        {
        case Report::Investors:
            nav->openReport(9, id, fromDate, toDate);
            break;
        default:
            break;
        }
    }
}
