#include "generalreport.h"
#include "pages/ui_generalreport.h"

GeneralReport::GeneralReport(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::GeneralReport)
{
    ui->setupUi(this);
    this->nav = nav;

    this->fromDate = QDate::currentDate().addDays(-1);
    this->toDate = QDate::currentDate();

    ui->FromDateButton->setProperty("color", "gray");
    ui->ToDateButton->setProperty("color", "gray");

    ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));

    connect(ui->tableView, &QTableView::doubleClicked, this, &GeneralReport::handleDoubleClick);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &GeneralReport::onSectionResized);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &GeneralReport::onSortIndicatorChanged);
}

GeneralReport::~GeneralReport()
{
    delete ui;
}

void GeneralReport::setReport(Report mode, QDate from, QDate to)
{
    //this->mode = mode;
    // this->fromDate = from;
    // this->toDate = to;

    // Показываем или скрываем кнопку в зависимости от типа отчёта
    if (mode == Report::Cars)
    {
        ui->ToPDFButtonSecond->setVisible(true);  // Показать кнопку
    }
    else
    {
        ui->ToPDFButtonSecond->setVisible(false); // Скрыть кнопку
    }

    this->selectedColumn = -1;

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
        ui->SecondReportButton->setText("ОБЩИЙ");
        break;

    case Report::Users2:
        ui->Header->setText("ПО КОЛИЧЕСТВУ ДЕЙСТВИЙ");
        ui->ReportButton->setText("ОТЧЕТ ПО ПОЛЬЗОВАТЕЛЮ");
        ui->SecondReportButton->setText("ДЕТАЛЬНО");
        break;

    case Report::Debts:
        ui->Header->setText("ПО ДОЛГАМ");
        ui->ReportButton->setText("ОТЧЕТ ПО МАШИНЕ");
        break;
    
    case Report::FinesByCars:
        ui->Header->setText("ПО ШТРАФАМ ПО МАШИНАМ");
        ui->ReportButton->setText("ОТЧЕТ ПО МАШИНЕ");
        break;
    
    case Report::FinesByDrivers:
        ui->Header->setText("ПО ШТРАФАМ ПО ВОДИТЕЛЯМ");
        ui->ReportButton->setText("ОТЧЕТ ПО ВОДИТЕЛЮ");
        break;

    // мои изменения
    case Report::ChargesByDrivers:
        ui->Header->setText("ПО ЗАРЯДКАМ ВОДИТЕЛЕЙ");
        ui->ReportButton->setText("ОТЧЕТ ПО ВОДИТЕЛЮ(необязательно)");
        break;
    }
}

void GeneralReport::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->mode)
    {
    case Report::Cars:
        model->setHorizontalHeaderLabels({"carId", "ID", "Инвестор", "Доход", "Налог 10%", "KWH x 10", "Расход", "Общий", "Дней", ">0", "Средняя", "%", "Комиссия", "Инвестору"});
        for (const QVariant &car : ReportOperations::getCarsReport(this->fromDate, this->toDate))
        {
            QVariantList cars = car.toList();
            QList<QStandardItem *> row;

            // Assume that the data in cars[] is in the correct order and types.
            row.append(new QStandardItem(cars[0].toString()));  // carId
            row.append(new QStandardItem(cars[1].toString()));  // carSid
            row.append(new QStandardItem(cars[2].toString()));  // investorName

            // Ensure numerical data is set correctly for sorting
            QStandardItem *incomeItem = new QStandardItem();
            incomeItem->setData(cars[3].toInt(), Qt::DisplayRole);
            row.append(incomeItem); // income

            QStandardItem *taxItem = new QStandardItem();
            taxItem->setData(cars[4].toInt(), Qt::DisplayRole);
            row.append(taxItem); // tax

            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(cars[5].toInt(), Qt::DisplayRole);
            row.append(kwhItem); // kwh * 10

            QStandardItem *outcomeItem = new QStandardItem();
            outcomeItem->setData(cars[6].toInt(), Qt::DisplayRole);
            row.append(outcomeItem); // outcome

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(cars[7].toInt(), Qt::DisplayRole);
            row.append(profitItem); // profit

            QStandardItem *daysItem = new QStandardItem();
            daysItem->setData(cars[8].toInt(), Qt::DisplayRole);
            row.append(daysItem); // daysWorked

            QStandardItem *daysItem2 = new QStandardItem();
            daysItem2->setData(cars[9].toInt(), Qt::DisplayRole);
            row.append(daysItem2); // nonZeroDays

            QStandardItem *averageItem = new QStandardItem();
            averageItem->setData(cars[10].toInt(), Qt::DisplayRole);
            row.append(averageItem); // averageProfitPerDay

            QStandardItem *percentageItem = new QStandardItem();
            percentageItem->setData(cars[11].toInt(), Qt::DisplayRole);
            row.append(percentageItem); // percentage

            QStandardItem *ourIncomeItem = new QStandardItem();
            ourIncomeItem->setData(cars[12].toInt(), Qt::DisplayRole);
            row.append(ourIncomeItem); // ourIncome

            QStandardItem *investorsIncomeItem = new QStandardItem();
            investorsIncomeItem->setData(cars[13].toInt(), Qt::DisplayRole);
            row.append(investorsIncomeItem); // investorsIncome

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
        model->setHorizontalHeaderLabels({"ID", "Имя", "Событий", "Доход", "KWH * 10", "Расход", "Оборот"});
        for (const QVariant &driver : ReportOperations::getDriversReport(this->fromDate, this->toDate))
        {
            QVariantList drivers = driver.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(drivers[0].toString()));  // ID
            row.append(new QStandardItem(drivers[1].toString()));  // Имя

            QStandardItem *eventsItem = new QStandardItem();
            eventsItem->setData(drivers[2].toInt(), Qt::DisplayRole);
            row.append(eventsItem); // Событий

            QStandardItem *incomeItem = new QStandardItem();
            incomeItem->setData(drivers[3].toInt(), Qt::DisplayRole);
            row.append(incomeItem); // Доход

            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(drivers[4].toInt(), Qt::DisplayRole);
            row.append(kwhItem); // kwh * 10

            QStandardItem *expenseItem = new QStandardItem();
            expenseItem->setData(drivers[5].toInt(), Qt::DisplayRole);
            row.append(expenseItem); // Расход

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(drivers[6].toInt(), Qt::DisplayRole);
            row.append(profitItem); // Прибыль

            model->appendRow(row);
        }
        break;
    case Report::Investors:
        model->setHorizontalHeaderLabels({"ID", "Имя", "Доход", "Налог 10%", "KWH x 10", "Расход", "Общий", "Комиссия", "Инвестору"});
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

            QStandardItem *taxItem = new QStandardItem();
            taxItem->setData(investors[3].toInt(), Qt::DisplayRole);
            row.append(taxItem); // Налог

            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(investors[4].toInt(), Qt::DisplayRole);
            row.append(kwhItem); // kwh * 10

            QStandardItem *expenseItem = new QStandardItem();
            expenseItem->setData(investors[5].toInt(), Qt::DisplayRole);
            row.append(expenseItem); // Расход

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(investors[6].toInt(), Qt::DisplayRole);
            row.append(profitItem); // Прибыль

            QStandardItem *ourMoneyItem = new QStandardItem();
            ourMoneyItem->setData(investors[7].toInt(), Qt::DisplayRole);
            row.append(ourMoneyItem); // Наша прибыль

            QStandardItem *investorsMoneyItem = new QStandardItem();
            investorsMoneyItem->setData(investors[8].toInt(), Qt::DisplayRole);
            row.append(investorsMoneyItem); // Прибыль инвестора

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


    // Мои изменения2
        case Report::ChargesByDrivers:
            model->setHorizontalHeaderLabels({"ID", "Водитель", "KWH"});
            for (const QVariant &driver : ReportOperations::getChargesByDriversReport(this->fromDate, this->toDate))
            {
                QVariantList drivers = driver.toList();
                QList<QStandardItem *> row;

                row.append(new QStandardItem(drivers[0].toString()));  // ID
                row.append(new QStandardItem(drivers[1].toString()));  // Имя

                QStandardItem *kwhItem = new QStandardItem();
                kwhItem->setData(drivers[2].toInt(), Qt::DisplayRole);  // Change to .toDouble() for float or double values
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
        model->setHorizontalHeaderLabels({"id", "Дата", "Машина", "Водитель", "Тип", "Сумма", "Пользователь"});
        for (const QVariant &user : ReportOperations::getUsersReport(this->fromDate, this->toDate))
        {
            QVariantList users = user.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(users[0].toString()));  // id
            row.append(new QStandardItem(users[1].toDateTime().toString("dd.MM.yyyy hh:mm:ss")));  // dateTime
            row.append(new QStandardItem(users[2].toString()));  // carId
            row.append(new QStandardItem(users[3].toString()));  // driverId
            row.append(new QStandardItem(users[4].toString()));  // typeId
            row.append(new QStandardItem()); // amount
            row[5]->setData(users[5].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem(users[6].toString()));  // userName

            model->appendRow(row);
        }
        break;

    case Report::Users2:
        if (true) {
            QVariantList data = ReportOperations::getUsers2Report(this->fromDate, this->toDate);
            QVariantList headerData = data[0].toList();
            QList<QString> header;
            header.append("0");
            for (int i = 0; i < headerData.size(); i++)
            {
                header.append(headerData[i].toString());
            }
            model->setHorizontalHeaderLabels(header);
            for (int i = 1; i < data.size(); i++)
            {
                QVariantList users = data[i].toList();

                QList<QStandardItem *> row;
                row.append(new QStandardItem("0"));
                row.append(new QStandardItem(users[0].toDate().toString("dd.MM.yyyy")));
                for (int j = 1; j < users.size(); j++)
                {
                    QStandardItem *item = new QStandardItem();
                    item->setData(users[j].toInt(), Qt::DisplayRole);
                    row.append(item);
                }
                model->appendRow(row);
            }
        }
        break;

    case Report::Debts:
        model->setHorizontalHeaderLabels({"id", "Машина", "Инвестор", "Количество", "Сумма"});
        for (const QVariant &debt : ReportOperations::getDebtsReport(this->fromDate, this->toDate))
        {
            QVariantList debts = debt.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(debts[0].toString()));  // id
            row.append(new QStandardItem(debts[1].toString()));  // carId
            row.append(new QStandardItem(debts[2].toString()));  // investor
            
            QStandardItem *rentCountItem = new QStandardItem();
            rentCountItem->setData(debts[3].toInt(), Qt::DisplayRole);
            row.append(rentCountItem); // rentCount

            QStandardItem *dentAmountItem = new QStandardItem();
            dentAmountItem->setData(debts[4].toInt(), Qt::DisplayRole);
            row.append(dentAmountItem); // dentAmount

            model->appendRow(row);
        }
        break;

    case Report::FinesByCars:
        model->setHorizontalHeaderLabels({"id", "Машина", "Сумма", "Количество", "Оплачено", "Количество", "Не оплачено", "Количество"});
        for (const QVariant &fine : ReportOperations::getFinesByCarsReport(this->fromDate, this->toDate))
        {
            QVariantList fines = fine.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(fines[0].toString()));  // id
            row.append(new QStandardItem(fines[1].toString()));  // carSid
            row.append(new QStandardItem());
            row[2]->setData(fines[2].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[3]->setData(fines[3].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[4]->setData(fines[4].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[5]->setData(fines[5].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[6]->setData(fines[6].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[7]->setData(fines[7].toInt(), Qt::DisplayRole);

            model->appendRow(row);
        }
        break;
    
    case Report::FinesByDrivers:
        model->setHorizontalHeaderLabels({"id", "Водитель", "Сумма", "Количество", "Оплачено", "Количество", "Не оплачено", "Количество"});
        for (const QVariant &fine : ReportOperations::getFinesByDriversReport(this->fromDate, this->toDate))
        {
            QVariantList fines = fine.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(fines[0].toString()));  // id
            row.append(new QStandardItem(fines[1].toString()));  // driverName
            row.append(new QStandardItem());
            row[2]->setData(fines[2].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[3]->setData(fines[3].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[4]->setData(fines[4].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[5]->setData(fines[5].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[6]->setData(fines[6].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[7]->setData(fines[7].toInt(), Qt::DisplayRole);

            model->appendRow(row);
        }
        break;

    //мои изменения
    // case Report::ChargesByDrivers:
    //     model->setHorizontalHeaderLabels({"Дата", "Водитель", "Машина", "KWH", "Время"});
    //     for (const QVariant &chargeData : ReportOperations::getChargesByDriverReport(0, this->fromDate, this->toDate))
    //     {
    //         QVariantList charge = chargeData.toList();
    //         QList<QStandardItem *> row;

    //         QStandardItem *dateItem = new QStandardItem();
    //         dateItem->setData(charge[0].toDateTime(), Qt::DisplayRole);
    //         row.append(dateItem);

    //         row.append(new QStandardItem(charge[1].toString())); // Водитель
    //         row.append(new QStandardItem(charge[2].toString())); // Машина

    //         QStandardItem *kwhItem = new QStandardItem();
    //         kwhItem->setData(charge[3].toInt(), Qt::DisplayRole); // KWH
    //         row.append(kwhItem);

    //         QStandardItem *timeItem = new QStandardItem();
    //         timeItem->setData(charge[4].toInt(), Qt::DisplayRole); // Время
    //         row.append(timeItem);

    //         model->appendRow(row);
    //     }
    //     break;






    default:
        break;
    }

    ui->tableView->setModel(model);

    ui->tableView->setColumnHidden(0, true);

    if (this->mode == Report::Users || this->mode == Report::Users2)
    {
        ui->SecondReportButton->setDisabled(false);
    }
    else
    {
        ui->SecondReportButton->setDisabled(true);
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    if (this->selectedColumn != -1)
        ui->tableView->sortByColumn(this->selectedColumn, this->sortOrder);
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
                                              "Доход",
                                              "Налог 10%",
                                              "KWH * 10",
                                              "Расход",
                                              "Общая",
                                              "Комиссия",
                                              "Инвесторам",
                                              });
            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(cars[0].toString());
            row << new QStandardItem(cars[1].toString());
            row << new QStandardItem(cars[2].toString());
            row << new QStandardItem(cars[3].toString());
            row << new QStandardItem(cars[4].toString());
            row << new QStandardItem(cars[5].toString());
            row << new QStandardItem(cars[6].toString());

            model->appendRow(row);
        }
        break;

    case Report::Types:
        for (const QVariant &type : ReportOperations::getAllTypesReport(this->fromDate, this->toDate))
        {
            QVariantList types = type.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              "Доход",
                                              "Расход",
                                              "Касса",
                                            });
            
            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(types[0].toString());
            row << new QStandardItem(types[1].toString());
            row << new QStandardItem(types[2].toString());
            model->appendRow(row);
        }
        break;

    case Report::Drivers:
        for (const QVariant &driver : ReportOperations::getAllDriversReport(this->fromDate, this->toDate))
        {
            QVariantList drivers = driver.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              "Событий",
                                              "Доход",
                                              "KWH * 10",
                                              "Расход",
                                              "Оборот",
                                              });
            
            QList<QStandardItem *> row;
            
            row << new QStandardItem("Итого");
            row << new QStandardItem(drivers[0].toString());
            row << new QStandardItem(drivers[1].toString());
            row << new QStandardItem(drivers[2].toString());
            row << new QStandardItem(drivers[3].toString());
            row << new QStandardItem(drivers[4].toString());
            
            model->appendRow(row);
        }
        break;

    case Report::Investors:
        for (const QVariant &investor : ReportOperations::getAllInvestorsReport(this->fromDate, this->toDate))
        {
            QVariantList investors = investor.toList();
            qDebug() << investors;
            model->setHorizontalHeaderLabels({"Итого",
                                              "Доход",
                                              "Налог 10%",
                                              "KWH * 10",
                                              "Расход",
                                              "Общая",
                                              "Комиссия",
                                              "Инвесторам",
                                              });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(investors[0].toString());
            row << new QStandardItem(investors[1].toString());
            row << new QStandardItem(investors[2].toString());
            row << new QStandardItem(investors[3].toString());
            row << new QStandardItem(investors[4].toString());
            row << new QStandardItem(investors[5].toString());
            row << new QStandardItem(investors[6].toString());
            model->appendRow(row);
        }
        break;

    case Report::Locations:
        for (const QVariant &location : ReportOperations::getAllLocationsReport(this->fromDate, this->toDate))
        {
            QVariantList locations = location.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "KWH",
            });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(locations[0].toString());
            model->appendRow(row);
        }
        break;
    //мои изменения2
    case Report::ChargesByDrivers:
        for (const QVariant &driver : ReportOperations::getAllChargesByDriversReport(this->fromDate, this->toDate))
        {
            QVariantList drivers = driver.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              "KWH",
                                              });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(drivers[0].toString());
            model->appendRow(row);
        }
        break;

        //     if (true)
        //     {
        //         QVariantList report = ReportOperations::getAllChargesByDriverReport(0, this->fromDate, this->toDate);
        //         model->setHorizontalHeaderLabels({"Итого", "KWH"});

        //         QList<QStandardItem *> row;
        //         row.append(new QStandardItem("Итого"));
        //         row.append(new QStandardItem(report[0].toString()));
        //         model->appendRow(row);
        //     }
        //     break;

    case Report::Charges:
        for (const QVariant &charge : ReportOperations::getAllChargesReport(this->fromDate, this->toDate))
        {
            QVariantList charges = charge.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "KWH",
            });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(charges[0].toString());
            model->appendRow(row);
        }
        break;

    case Report::Users:
        for (const QVariant &user : ReportOperations::getAllUsersReport(this->fromDate, this->toDate))
        {
            QVariantList users = user.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "Входов",
                "Событий",
            });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(users[0].toString());
            row << new QStandardItem(users[1].toString());
            model->appendRow(row);
        }
        break;
    
    case Report::Users2:
        if (true) {
            QVariantList report = ReportOperations::getAllUsers2Report(this->fromDate, this->toDate);
            QVariantList users = report[0].toList();
            QList<QString> data;
            data << "Итого";
            for (int i = 0; i < users.size(); i++)
            {
                data << users[i].toString();
            }
            model->setHorizontalHeaderLabels(data);
            for (int i = 1; i < report.size(); i++)
            {
                QList<QStandardItem *> row;
                row << new QStandardItem("Итого");
                QVariantList users = report[i].toList();
                for (int j = 0; j < users.size(); j++) 
                {
                    row << new QStandardItem(users[j].toString());
                }
                model->appendRow(row);
            }
        }
        break;

    case Report::Debts:
        for (const QVariant &debt : ReportOperations::getAllDebtsReport(this->fromDate, this->toDate))
        {
            QVariantList debts = debt.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "Долгов",
                "Сумма",
            });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(debts[0].toString());
            row << new QStandardItem(debts[1].toString());
            model->appendRow(row);
        }
        break;

    case Report::FinesByCars:
        for (const QVariant &fine : ReportOperations::getAllFinesByCarsReport(this->fromDate, this->toDate))
        {
            QVariantList fines = fine.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "Сумма",
                "Количество",
                "Оплачено",
                "Количество",
                "Не оплачено",
                "Количество",
            });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(fines[0].toString());
            row << new QStandardItem(fines[1].toString());
            row << new QStandardItem(fines[2].toString());
            row << new QStandardItem(fines[3].toString());
            row << new QStandardItem(fines[4].toString());
            row << new QStandardItem(fines[5].toString());
            model->appendRow(row);
        }
        break;
    
    case Report::FinesByDrivers:
        for (const QVariant &fine : ReportOperations::getAllFinesByDriversReport(this->fromDate, this->toDate))
        {
            QVariantList fines = fine.toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "Сумма",
                "Количество",
                "Оплачено",
                "Количество",
                "Не оплачено",
                "Количество",
            });

            QList<QStandardItem *> row;

            row << new QStandardItem("Итого");
            row << new QStandardItem(fines[0].toString());
            row << new QStandardItem(fines[1].toString());
            row << new QStandardItem(fines[2].toString());
            row << new QStandardItem(fines[3].toString());
            row << new QStandardItem(fines[4].toString());
            row << new QStandardItem(fines[5].toString());
            model->appendRow(row);
        }
        break;
    //мои изменения
    // case Report::ChargesByDrivers:
    //     if (true)
    //     {
    //         QVariantList report = ReportOperations::getAllChargesByDriverReport(0, this->fromDate, this->toDate);
    //         model->setHorizontalHeaderLabels({"Итого", "KWH"});

    //         QList<QStandardItem *> row;
    //         row.append(new QStandardItem("Итого"));
    //         row.append(new QStandardItem(report[0].toString()));
    //         model->appendRow(row);
    //     }
    //     break;



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
        ui->tableView->setColumnWidth(1, 100);
        ui->tableView->setColumnWidth(2, 120);
        ui->tableView->setColumnWidth(3, 130);
        ui->tableView->setColumnWidth(4, 120);
        ui->tableView->setColumnWidth(5, 130);
        ui->tableView->setColumnWidth(6, 130);
        ui->tableView->setColumnWidth(7, 130);
        ui->tableView->setColumnWidth(8, 60);
        ui->tableView->setColumnWidth(9, 50);
        ui->tableView->setColumnWidth(10, 130);
        ui->tableView->setColumnWidth(11, 50);
        ui->tableView->setColumnWidth(12, 130);
        ui->tableView->setColumnWidth(13, 130);
        break;

    case Report::Types:
    case Report::Debts:
        ui->tableView->setColumnWidth(1, 226);
        ui->tableView->setColumnWidth(2, 226);
        ui->tableView->setColumnWidth(3, 226);
        ui->tableView->setColumnWidth(4, 226);
        break;

    case Report::Drivers:
        ui->tableView->setColumnWidth(1, 210);
        ui->tableView->setColumnWidth(2, 210);
        ui->tableView->setColumnWidth(3, 210);
        ui->tableView->setColumnWidth(4, 210);
        ui->tableView->setColumnWidth(5, 210);
        break;

    case Report::Investors:
        ui->tableView->setColumnWidth(1, 180);
        ui->tableView->setColumnWidth(2, 180);
        ui->tableView->setColumnWidth(3, 140);
        ui->tableView->setColumnWidth(4, 180);
        ui->tableView->setColumnWidth(5, 180);
        ui->tableView->setColumnWidth(6, 180);
        ui->tableView->setColumnWidth(7, 180);
        break;

    case Report::Locations:
        ui->tableView->setColumnWidth(1, 377);
        ui->tableView->setColumnWidth(2, 377);
        break;

        //мои изменения2
    case Report::ChargesByDrivers:
        ui->tableView->setColumnWidth(1, 377);
        ui->tableView->setColumnWidth(2, 377);
        break;

    case Report::Charges:
        ui->tableView->setColumnWidth(1, 377);
        ui->tableView->setColumnWidth(2, 377);
        break;

    case Report::Users:
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 200);
        ui->tableView->setColumnWidth(6, 200);
        break;

    case Report::Users2:
        if (true)
        {
            int size = 1300 / ui->tableView->model()->columnCount();
            for (int i = 0; i < ui->tableView->model()->columnCount(); i++)
            {
                ui->tableView->setColumnWidth(i, size);
            }
        }

    case Report::FinesByDrivers:
    case Report::FinesByCars:
        ui->tableView->setColumnWidth(1, 190);
        ui->tableView->setColumnWidth(2, 190);
        ui->tableView->setColumnWidth(3, 190);
        ui->tableView->setColumnWidth(4, 190);
        ui->tableView->setColumnWidth(5, 190);
        ui->tableView->setColumnWidth(6, 190);
        ui->tableView->setColumnWidth(7, 190);

    // мои изменения
    // case Report::ChargesByDrivers:
    //     ui->tableView->setColumnWidth(0, 172);
    //     ui->tableView->setColumnWidth(1, 172);
    //     ui->tableView->setColumnWidth(2, 172);
    //     ui->tableView->setColumnWidth(3, 172);
    //     ui->tableView->setColumnWidth(4, 172);
    //     break;


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
    case Report::Debts:
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
    case Report::Users2:
        nav->openSettings(5);
        break;
    case Report::FinesByDrivers:
    case Report::FinesByCars:
        nav->openFines(0);
        break;
    // мои изменения
    case Report::ChargesByDrivers:
        nav->openSettings(0);
        break;

    default:
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
        case Report::Debts:
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
        case Report::Users2:
            nav->openReport(11, id, fromDate, toDate);
            break;
        case Report::FinesByCars:
            nav->openFines(3, id, fromDate, toDate);
            break;
        case Report::FinesByDrivers:
            nav->openFines(4, id, fromDate, toDate);
            break;
        // мои изменения
        case Report::ChargesByDrivers:
            nav->openReport(17, id, fromDate, toDate);
            break;

        default:
            break;
        }
    }
    else
    {
        switch (this->mode)
        {
        case Report::Cars:
        case Report::Debts:
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
        case Report::Users2:
            nav->openReport(11, 0, fromDate, toDate);
            break;
        case Report::FinesByCars:
            nav->openFines(3, 0, fromDate, toDate);
            break;
        case Report::FinesByDrivers:
            nav->openFines(4, 0, fromDate, toDate);
            break;
        case Report::ChargesByDrivers:
            nav->openReport(17, 0, fromDate, toDate);
            break;
        default:
            break;
        }
    }
}

void GeneralReport::on_SecondReportButton_clicked()
{
    switch (this->mode)
    {
    case Report::Users:
        nav->openReport(5, 0, fromDate, toDate);
        break;
    case Report::Users2:
        nav->openReport(14, 0, fromDate, toDate);
        break;
    default:
        break;
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

// void GeneralReport::on_ToPDFButtonSecond_clicked()
// {
//     // Получаем даты из виджетов
//     QDate fromDate = this->fromDate;
//     QDate toDate = this->toDate;

//     // Проверка корректности дат
//     if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
//     {
//         QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
//         return;
//     }

//     QList<QStandardItemModel*> models;

//     // Проходим по каждой дате в указанном диапазоне
//     for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
//     {
//         QStandardItemModel *model = new QStandardItemModel();
//         model->setHorizontalHeaderLabels({"Итого", "Доход", "Налог 10%", "KWH * 10", "Расход", "Общая", "Комиссия", "Инвесторам"});

//         switch (this->mode)
//         {
//         case Report::Cars:
//             for (const QVariant &car : ReportOperations::getAllCarsReport(date, date))
//             {
//                 QVariantList cars = car.toList();
//                 QList<QStandardItem *> row;

//                 row << new QStandardItem("Итого");
//                 row << new QStandardItem(cars[0].toString());
//                 row << new QStandardItem(cars[1].toString());
//                 row << new QStandardItem(cars[2].toString());
//                 row << new QStandardItem(cars[3].toString());
//                 row << new QStandardItem(cars[4].toString());
//                 row << new QStandardItem(cars[5].toString());
//                 row << new QStandardItem(cars[6].toString());

//                 model->appendRow(row);
//             }
//             break;

//             // Добавьте другие типы отчётов, если требуется

//         default:
//             break;
//         }

//         // Добавляем модель в список моделей
//         if (model->rowCount() > 0)
//             models.append(model);
//     }

//     if (models.isEmpty())
//     {
//         QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
//         return;
//     }

//     // Генерируем PDF-файл с использованием нового класса PDFmanagerSecond
//     PDFmanagerSecond::exportToPDFByDays("Отчет по дням", fromDate, toDate, models);

//     // Освобождаем память
//     qDeleteAll(models);
// }

// void GeneralReport::on_ToPDFButtonSecond_clicked()
// {
//     // Получаем даты из виджетов
//     QDate fromDate = this->fromDate;
//     QDate toDate = this->toDate;

//     // Проверка корректности дат
//     if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
//     {
//         QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
//         return;
//     }

//     QList<QStandardItemModel*> models;

//     // Проходим по каждой дате в указанном диапазоне
//     for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
//     {
//         QStandardItemModel *model = new QStandardItemModel();
//         model->setHorizontalHeaderLabels({"Итого", "Доход", "Налог 10%", "KWH * 10", "Расход", "Общая", "Комиссия", "Инвесторам"});

//         switch (this->mode)
//         {
//         case Report::Cars:
//             // Теперь данные берутся ТОЛЬКО за текущую дату
//             for (const QVariant &car : ReportOperations::getAllCarsReport(date, date))
//             {
//                 QVariantList cars = car.toList();
//                 QList<QStandardItem *> row;

//                 row << new QStandardItem("Итого");
//                 row << new QStandardItem(cars[0].toString());
//                 row << new QStandardItem(cars[1].toString());
//                 row << new QStandardItem(cars[2].toString());
//                 row << new QStandardItem(cars[3].toString());
//                 row << new QStandardItem(cars[4].toString());
//                 row << new QStandardItem(cars[5].toString());
//                 row << new QStandardItem(cars[6].toString());

//                 model->appendRow(row);
//             }
//             break;

//         default:
//             break;
//         }

//         // Добавляем модель в список моделей ТОЛЬКО если есть данные за конкретную дату
//         if (model->rowCount() > 0)
//             models.append(model);
//         else
//             delete model;  // Освобождаем память для пустых моделей
//     }

//     if (models.isEmpty())
//     {
//         QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
//         return;
//     }

//     // Генерируем PDF-файл с использованием нового класса PDFmanagerSecond
//     PDFmanagerSecond::exportToPDFByDays("Отчет по дням", fromDate, toDate, models);

//     // Освобождаем память
//     qDeleteAll(models);
// }


// void GeneralReport::on_ToPDFButtonSecond_clicked()
// {
//     QDate fromDate = this->fromDate;
//     QDate toDate = this->toDate;

//     if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
//     {
//         QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
//         return;
//     }

//     QMap<QDate, QList<QStandardItemModel*>> dateModels;

//     // Проходим по каждой дате и собираем все модели в список
//     for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
//     {
//         QList<QStandardItemModel*> models;

//         switch (this->mode)
//         {
//         case Report::Cars:
//             for (const QVariant &car : ReportOperations::getAllCarsReport(date, date))
//             {
//                 QVariantList cars = car.toList();
//                 QStandardItemModel* model = new QStandardItemModel();
//                 model->setHorizontalHeaderLabels({"Итого", "Доход", "Налог 10%", "KWH * 10", "Расход", "Общая", "Комиссия", "Инвесторам"});

//                 QList<QStandardItem *> row;
//                 row << new QStandardItem("Итого");
//                 row << new QStandardItem(cars[0].toString());
//                 row << new QStandardItem(cars[1].toString());
//                 row << new QStandardItem(cars[2].toString());
//                 row << new QStandardItem(cars[3].toString());
//                 row << new QStandardItem(cars[4].toString());
//                 row << new QStandardItem(cars[5].toString());
//                 row << new QStandardItem(cars[6].toString());

//                 model->appendRow(row);
//                 models.append(model);
//             }
//             break;

//         default:
//             break;
//         }

//         if (!models.isEmpty())
//         {
//             dateModels.insert(date, models);  // Добавляем список моделей для этой даты
//         }
//     }

//     if (dateModels.isEmpty())
//     {
//         QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
//         return;
//     }

//     PDFmanagerSecond::exportToPDFByDays("Отчет по дням", dateModels, 1);

//     // Освобождаем память
//     // for (auto models : dateModels)
//     // {
//     //     qDeleteAll(models);
//     // }
// }

//pdf по дням, создает нужное количество страниц, но показывает только один столбик
// void GeneralReport::on_ToPDFButtonSecond_clicked()
// {
//     QDate fromDate = this->fromDate;
//     QDate toDate = this->toDate;

//     if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
//     {
//         QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
//         return;
//     }

//     QMap<QDate, QList<QStandardItemModel *>> dateModels;

//     for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
//     {
//         QList<QStandardItemModel *> modelsForDate;

//         QStandardItemModel *model = new QStandardItemModel();
//         model->setHorizontalHeaderLabels({"Итого", "Доход", "Налог 10%", "KWH * 10", "Расход", "Общая", "Комиссия", "Инвесторам"});

//         for (const QVariant &car : ReportOperations::getAllCarsReport(date, date))
//         {
//             QVariantList cars = car.toList();
//             QList<QStandardItem *> row;

//             row << new QStandardItem("Итого");
//             row << new QStandardItem(cars[0].toString());
//             row << new QStandardItem(cars[1].toString());
//             row << new QStandardItem(cars[2].toString());
//             row << new QStandardItem(cars[3].toString());
//             row << new QStandardItem(cars[4].toString());
//             row << new QStandardItem(cars[5].toString());
//             row << new QStandardItem(cars[6].toString());

//             model->appendRow(row);
//         }

//         if (model->rowCount() > 0)
//             modelsForDate.append(model);

//         if (!modelsForDate.isEmpty())
//             dateModels.insert(date, modelsForDate);
//     }

//     if (dateModels.isEmpty())
//     {
//         QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
//         return;
//     }

//     PDFmanagerSecond::exportToPDFByDays("Отчет по дням", fromDate, toDate, dateModels);

//     for (auto models : dateModels)
//         qDeleteAll(models);
// }



// void GeneralReport::on_ToPDFButtonSecond_clicked()
// {
//     // Получаем даты из виджетов
//     QDate fromDate = this->fromDate;
//     QDate toDate = this->toDate;

//     // Проверка корректности дат
//     if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
//     {
//         QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
//         return;
//     }

//     QMap<QDate, QStandardItemModel*> dateModels;

//     // ✅ Цикл для добавления всех строк в модель
//     for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
//     {
//         QStandardItemModel *model = new QStandardItemModel();
//         model->setHorizontalHeaderLabels({"#", "Доход", "Налог 10%", "KWH * 10", "Расход", "Общая", "Комиссия", "Инвесторам"});

//         int rowCount = 1;

//         switch (this->mode)
//         {
//         case Report::Cars:
//             for (const QVariant &car : ReportOperations::getAllCarsReport(date, date))
//             {
//                 QVariantList cars = car.toList();
//                 QList<QStandardItem *> row;

//                 // Добавляем номер строки
//                 row << new QStandardItem(QString::number(rowCount++));
//                 row << new QStandardItem(cars[0].toString());
//                 row << new QStandardItem(cars[1].toString());
//                 row << new QStandardItem(cars[2].toString());
//                 row << new QStandardItem(cars[3].toString());
//                 row << new QStandardItem(cars[4].toString());
//                 row << new QStandardItem(cars[5].toString());
//                 row << new QStandardItem(cars[6].toString());

//                 model->appendRow(row);
//             }
//             break;

//         default:
//             break;
//         }

//         if (model->rowCount() > 0)
//             dateModels.insert(date, model);
//         else
//             delete model;
//     }

//     if (dateModels.isEmpty())
//     {
//         QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
//         return;
//     }

//     // ✅ Вызов второго фрагмента кода для генерации PDF
//     PDFmanagerSecond::exportToPDFByDays("Отчет по дням", dateModels, 1);

//     qDeleteAll(dateModels);
// }



// void GeneralReport::on_ToPDFButtonSecond_clicked()
// {
//     // Получаем даты из виджетов
//     QDate fromDate = this->fromDate;
//     QDate toDate = this->toDate;

//     if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
//     {
//         QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
//         return;
//     }

//     QMap<QDate, QStandardItemModel*> dateModels;

//     // Проходим по каждой дате в указанном диапазоне
//     for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
//     {
//         QStandardItemModel *model = new QStandardItemModel();
//         model->setHorizontalHeaderLabels({"ID", "Инвестор", "Доход", "Налог 10%", "KWH x 10", "Расход", "Общий", "Дней", ">0", "Средняя", "%", "Комиссия", "Инвестору"});




//         switch (this->mode)
//         {
//         case Report::Cars:
//             for (const QVariant &car : ReportOperations::getCarsReport(date, date))
//             {
//                 QVariantList cars = car.toList();

//                 //комментировано
//                 QList<QStandardItem *> row;

//                 row.append(new QStandardItem(cars[1].toString()));  // ID
//                 row.append(new QStandardItem(cars[2].toString()));  // Инвестор

//                 for (int i = 3; i <= 13; ++i) // Добавляем остальные числовые значения
//                 {
//                     QStandardItem *item = new QStandardItem();
//                     item->setData(cars[i].toInt(), Qt::DisplayRole);
//                     row.append(item);
//                 }

//                 model->appendRow(row);
//             }
//             break;

//         default:
//             break;
//         }

//         if (model->rowCount() > 0)
//             dateModels.insert(date, model);
//     }

//     if (dateModels.isEmpty())
//     {
//         QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
//         return;
//     }

//     // Генерируем PDF-файл с использованием PDFmanagerSecond
//     PDFmanagerSecond::exportToPDFByDays("Отчет по дням", dateModels, 1);

//     // Освобождаем память
//     qDeleteAll(dateModels);
// }


void GeneralReport::on_ToPDFButtonSecond_clicked()
{
    // Получаем даты из виджетов
    QDate fromDate = this->fromDate;
    QDate toDate = this->toDate;

    // Проверка корректности дат
    if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
    {
        QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
        return;
    }

    QMap<QDate, QStandardItemModel*> dateModels;

    // Проходим по каждой дате в указанном диапазоне
    for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
    {
        QStandardItemModel *model = new QStandardItemModel();
        model->setHorizontalHeaderLabels({"#", "Инвестор", "Доход", "Налог 10%", "KWH x 10", "Расход", "Общий", "Дней", ">0", "Средняя", "%", "Комиссия", "Инвестору"});

        int rowNumber = 1; // Для нумерации строк

        switch (this->mode)
        {
        case Report::Cars:
            for (const QVariant &car : ReportOperations::getCarsReport(date, date))
            {
                QVariantList cars = car.toList();

                // Проверка на наличие значений в ключевых колонках
                int income = cars[3].toInt();
                int tax = cars[4].toInt();
                int kwh = cars[5].toInt();
                int outcome = cars[6].toInt();
                int profit = cars[7].toInt();

                if (income != 0 || tax != 0 || kwh != 0 || outcome != 0 || profit != 0)  // ✅ Добавили проверку
                {
                    QList<QStandardItem *> row;

                    row.append(new QStandardItem(QString::number(rowNumber++))); // #
                    row.append(new QStandardItem(cars[2].toString())); // Инвестор
                    row.append(new QStandardItem(QString::number(income))); // Доход
                    row.append(new QStandardItem(QString::number(tax))); // Налог 10%
                    row.append(new QStandardItem(QString::number(kwh))); // KWH x 10
                    row.append(new QStandardItem(QString::number(outcome))); // Расход
                    row.append(new QStandardItem(QString::number(profit))); // Общий
                    row.append(new QStandardItem(QString::number(cars[8].toInt()))); // Дней
                    row.append(new QStandardItem(QString::number(cars[9].toInt()))); // >0
                    row.append(new QStandardItem(QString::number(cars[10].toInt()))); // Средняя
                    row.append(new QStandardItem(QString::number(cars[11].toInt()))); // %
                    row.append(new QStandardItem(QString::number(cars[12].toInt()))); // Комиссия
                    row.append(new QStandardItem(QString::number(cars[13].toInt()))); // Инвестору

                    model->appendRow(row);
                }
            }
            break;

        default:
            break;
        }

        // Добавляем модель в список моделей, если есть хотя бы одна строка
        if (model->rowCount() > 0)
            dateModels.insert(date, model);
        else
            delete model; // Удаляем пустую модель
    }

    if (dateModels.isEmpty())
    {
        QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
        return;
    }

    // Генерируем PDF-файл с использованием нового класса PDFmanagerSecond
    PDFmanagerSecond::exportToPDFByDays("Отчет по дням", dateModels);

    // Освобождаем память
    qDeleteAll(dateModels);
}



// void GeneralReport::on_ToPDFButtonSecond_clicked()
// {
//     // Получаем даты из виджетов
//     QDate fromDate = this->fromDate;
//     QDate toDate = this->toDate;

//     // Проверка корректности дат
//     if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate)
//     {
//         QMessageBox::warning(this, "Ошибка", "Некорректный диапазон дат.");
//         return;
//     }

//     QMap<QDate, QList<QStandardItemModel *>> dateModels;

//     // Проходим по каждой дате в указанном диапазоне
//     for (QDate date = fromDate; date <= toDate; date = date.addDays(1))
//     {
//         QList<QStandardItemModel *> models;

//         switch (this->mode)
//         {
//         case Report::Cars:
//         {
//             int rowNumber = 1; // Нумерация строк в таблице для каждой даты
//             for (const QVariant &car : ReportOperations::getCarsReport(date, date))
//             {
//                 QVariantList cars = car.toList();

//                 // Проверка на наличие значений в ключевых колонках
//                 int income = cars[3].toInt();
//                 int tax = cars[4].toInt();
//                 int kwh = cars[5].toInt();
//                 int outcome = cars[6].toInt();
//                 int profit = cars[7].toInt();

//                 if (income != 0 || tax != 0 || kwh != 0 || outcome != 0 || profit != 0)
//                 {
//                     QStandardItemModel *model = new QStandardItemModel();
//                     model->setHorizontalHeaderLabels({"#", "Инвестор", "Доход", "Налог 10%", "KWH x 10",
//                                                       "Расход", "Общий", "Дней", ">0", "Средняя", "%",
//                                                       "Комиссия", "Инвестору"});

//                     QList<QStandardItem *> row;
//                     row.append(new QStandardItem(QString::number(rowNumber++)));  // #
//                     row.append(new QStandardItem(cars[2].toString()));            // Инвестор
//                     row.append(new QStandardItem(QString::number(income)));      // Доход
//                     row.append(new QStandardItem(QString::number(tax)));         // Налог 10%
//                     row.append(new QStandardItem(QString::number(kwh)));         // KWH x 10
//                     row.append(new QStandardItem(QString::number(outcome)));     // Расход
//                     row.append(new QStandardItem(QString::number(profit)));      // Общий
//                     row.append(new QStandardItem(QString::number(cars[8].toInt())));  // Дней
//                     row.append(new QStandardItem(QString::number(cars[9].toInt())));  // >0
//                     row.append(new QStandardItem(QString::number(cars[10].toInt()))); // Средняя
//                     row.append(new QStandardItem(QString::number(cars[11].toInt()))); // %
//                     row.append(new QStandardItem(QString::number(cars[12].toInt()))); // Комиссия
//                     row.append(new QStandardItem(QString::number(cars[13].toInt()))); // Инвестору

//                     model->appendRow(row);
//                     models.append(model); // Добавляем модель в список моделей для этой даты
//                 }
//             }
//         }
//         break;

//         default:
//             break;
//         }

//         if (!models.isEmpty())
//             dateModels.insert(date, models); // Добавляем список моделей для даты в map
//     }

//     if (dateModels.isEmpty())
//     {
//         QMessageBox::information(this, "Информация", "Нет данных для выбранного диапазона дат.");
//         return;
//     }

//     // Генерируем PDF-файл с использованием нового класса PDFmanagerSecond
//     PDFmanagerSecond::exportToPDFByDays("Отчет по дням", dateModels, fromDate, toDate, 1);

//     // Освобождаем память
//     for (auto &models : dateModels)
//         qDeleteAll(models);
// }







void GeneralReport::on_ToPDFButton_clicked()
{
    QString title;
    switch (this->mode)
    {
    case Report::Cars:
        title = "Отчет по машинам";
        break;

    case Report::Drivers:
        title = "Отчет по водителям";
        break;

    case Report::Investors:
        title = "Отчет по инвесторам";
        break;

    case Report::Types:
        title = "Отчет по типам";
        break;

    case Report::Locations:
        title = "Отчет по локациям";
        break;

    case Report::Charges:
        title = "Отчет по зарядкам";
        break;

    case Report::Users:
        title = "Отчет по пользователям";
        break;

    case Report::Users2:
        title = "Отчет по количеству действий";
        break;
    
    case Report::Debts:
        title = "Отчет по долгам";
        break;
    
    case Report::FinesByDrivers:
        title = "Отчет по штрафам по водителям";
        break;
    
    case Report::FinesByCars:
        title = "Отчет по штрафам по машинам";
        break;
    // мои изменения
    case Report::ChargesByDrivers:
        title = "Отчет по зарядкам водителей";
        break;


    default:
        break;
    }
    PDFmanager::exportToPDF(title, this->fromDate.toString("dd.MM.yyyy") + " - " + this->toDate.toString("dd.MM.yyyy"), { ui->tableView->model(), ui->bottomTable->model() });
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

void GeneralReport::handleDoubleClick(const QModelIndex &index)
{
    switch (this->mode)
    {
        case Report::Cars:
        if (true) {
            QVariant data = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0));
            Car car = Operations::getCar(data.toLongLong());
            QString text = "ID: " + car.getSid() + "\n"
                           "Марка: " + car.getBrand() + "\n"
                           "Модель: " + car.getModel() + "\n"
                           "Госномер: " + car.getLicensePlate() + "\n"
                           "Год: " + QString::number(car.getYear()) + "\n"
                           "Инвестор: " + Operations::getInvestor(car.getInvestorId()).getName() + "\n"
                           "Пробег: " + QString::number(car.getMilleage()) + "\n"
                           "Описание: " + car.getDescription() + "\n";
            QMessageBox::information(this, "Информация о машине", text);
        }
            break;

        default:
            break;
    }
}

void GeneralReport::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order) {
    this->selectedColumn = logicalIndex;
    this->sortOrder = order;
}
