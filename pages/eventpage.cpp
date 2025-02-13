#include "eventpage.h"
#include "ui_eventpage.h"

eventPage::eventPage(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::eventPage)
{
    ui->setupUi(this);
    this->nav = nav;

    this->date = QDate::currentDate();

    userSession &u = userSession::getInstance();

    if (!u.checkIsAdmin())
    {
        ui->ReportButton->setDisabled(true);
    }

    ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));

    ui->DateButton->setProperty("color", "gray");
}

eventPage::~eventPage()
{
    delete ui;
}

void eventPage::setEvents(Events table, QDate date)
{
    this->table = table;

    if (!date.isNull())
    {
        this->date = date;

        ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));
    }

    setHeader();
    setTable();
    setBottomTable();
}

void eventPage::setHeader()
{
    QString header = "bark";
    switch (this->table)
    {
    case Events::Events:
        header = "СОБЫТИЯ";
        break;

    case Events::Charges:
        header = "ЗАРЯДКИ";
        break;
    }

    ui->EcoTaxi->setText(header);
}

bool eventPage::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->table)
    {
    case Events::Events:
        model->setHorizontalHeaderLabels({"id", "Время", "Тип", "ID Водителя", "ID Машины", "Сумма", "Описание"});
        for (Event event : Operations::selectEventsByDate(this->date))
        {
            QString driverShow = "-";
            if (event.getDriverId() > 0)
            {
                Driver driver = Operations::getDriver(event.getDriverId());
                driverShow = driver.getName();
            }

            QString carShow = "-";
            if (event.getCarId() > 0)
            {
                Car car = Operations::getCar(event.getCarId());
                carShow = QString::number(car.getSid());
            }
            model->appendRow({new QStandardItem(QString::number(event.getId())), new QStandardItem(event.getDate().toString("HH:mm:ss")), new QStandardItem(Operations::getType(event.getTypeId()).getName()), new QStandardItem(driverShow), new QStandardItem(carShow), new QStandardItem(QString::number(event.getAmount())), new QStandardItem(event.getDescription())});
        }
        break;
    case Events::Charges:
        model->setHorizontalHeaderLabels({"id", "Время", "ID Машины", "ID Водителя", "Локация", "КВТ", "Время"});
        for (Charge charge : Operations::selectChargesByDate(this->date))
        {
            Driver driver = Operations::getDriver(charge.getDriverId());
            Location location = Operations::getLocation(charge.getLocationId());
            QString time = (charge.getDuration() != 0) ? QString::number(charge.getDuration()) : "-";
            model->appendRow({new QStandardItem(QString::number(charge.getId())), new QStandardItem(charge.getDate().toString("HH:mm:ss")), new QStandardItem(QString::number(Operations::getCar(charge.getCarId()).getSid())), new QStandardItem(driver.getName()), new QStandardItem(location.getName()), new QStandardItem(QString::number(charge.getKwh())), new QStandardItem(time)});
        }
    }
    ui->tableView->setModel(model);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &eventPage::onSectionResized);

    switch (this->table)
    {
    case Events::Events:
        ui->tableView->setColumnWidth(1, 120);
        ui->tableView->setColumnWidth(2, 199);
        ui->tableView->setColumnWidth(3, 199);
        ui->tableView->setColumnWidth(4, 199);
        ui->tableView->setColumnWidth(5, 199);
        ui->tableView->setColumnWidth(6, 215);
        break;

    case Events::Charges:
        ui->tableView->setColumnWidth(1, 120);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 200);
        ui->tableView->setColumnWidth(6, 215);
        break;
    }

    return true;
}


void eventPage::setBottomTable() {

    QStandardItemModel *model = new QStandardItemModel();

    switch (this->table)
    {
    case Events::Events:
        for (const QVariant &event : Operations::getAllEventsReport(this->date)) {
            QVariantList events = event.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              events[0].toString()});
        }
        break;

    case Events::Charges:
        for (const QVariant &charge : Operations::getAllChargesReport(this->date)) {
            QVariantList charges = charge.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              charges[0].toString()});
        }
        break;

    default:
        break;
    }

    ui->bottomTable->setModel(model);

    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void eventPage::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void eventPage::on_DeleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        QString question;

        switch (this->table)
        {
        case Events::Events:
            if (true)
            {
                Event event = Operations::getEvent(id);
                question = "Вы уверены что хотите удалить эвент " + QString::number(event.getId()) + ", " + event.getDate().toString("HH:mm:ss") + ", " + Operations::getType(event.getTypeId()).getName() + ", " + QString::number(event.getAmount()) + ", " + event.getDescription() + "?";
            }
            break;
        case Events::Charges:
            if (true)
            {
                Charge charge = Operations::getCharge(id);
                question = "Вы уверены что хотите удалить зарядку " + QString::number(charge.getId()) + ", " + charge.getDate().toString("HH:mm:ss") + ", " + QString::number(Operations::getCar(charge.getCarId()).getSid()) + ", " + Operations::getDriver(charge.getDriverId()).getName() + ", " + Operations::getLocation(charge.getLocationId()).getName() + ", " + QString::number(charge.getKwh()) + ", " + QString::number(charge.getDuration()) + "?";
            }
        default:
            break;
        }
        CustomDialog *d = new CustomDialog(question);

        connect(d, &CustomDialog::yesClicked, this, &eventPage::onYes);
        connect(d, &CustomDialog::noClicked, this, &eventPage::onNo);

        d->show();
    }
}

void eventPage::onYes()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->table)
        {
        case Events::Events:
            Operations::deleteEvent(id);
            break;
        case Events::Charges:
            Operations::deleteCharge(id);
            break;
        }
        setTable();
    }
}

void eventPage::onNo()
{
    // nav->changeWindow(0);
}

void eventPage::on_EditButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        addupdatewindowEvents *w = new addupdatewindowEvents(this->table, id);
        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}

void eventPage::on_DateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->date);

    connect(c, &CalendarPage::changeDate, this, &eventPage::onDateChange);

    c->show();
}

void eventPage::onDateChange(QDate date)
{
    this->date = date;
    ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));
    setTable();
    setBottomTable();
}

void eventPage::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}

void eventPage::adjustColumnWidths()
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

void eventPage::on_ReportButton_clicked()
{
    switch (this->table)
    {
    case Events::Events:
        nav->openReport(1);
        break;
    case Events::Charges:
        nav->openReport(6);
        break;
    }
}
