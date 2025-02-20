#include "editablereport.h"
#include "ui_editablereport.h"

EditableReport::EditableReport(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditableReport)
{
    ui->setupUi(this);
    this->nav = nav;
}

void EditableReport::openReport(eSetting mode)
{
    this->mode = mode;
    switch (this->mode) {
    case eSetting::Repairs:
        ui->EcoTaxi->setText("РЕМОНТ");
        break;
    case eSetting::Fines:
        ui->EcoTaxi->setText("ШТРАФЫ");
        break;
    default:
        break;
    }
    setTable();
    setTableSize();
}

EditableReport::~EditableReport()
{
    delete ui;
}

void EditableReport::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    switch (this->mode) {
    case eSetting::Repairs:
        if (true)
        {
            model->setHorizontalHeaderLabels({ "ID", "Машина", "Инвестор", "Дней", "От", "До", "Описание" });
            QVariantList report;
            if (ui->checkBox->isChecked())
                report = ReportOperations::getNotDoneRepairsReport();
            else
                report = ReportOperations::getRepairsReport();
            for (QVariant &repair: report)
            {
                QVariantList rp = repair.toList();
                QList<QStandardItem *> row;

                row << new QStandardItem(rp[0].toString());
                row << new QStandardItem(rp[1].toString());
                row << new QStandardItem(rp[2].toString());

                QStandardItem *item = new QStandardItem();
                item->setData(rp[3].toInt(), Qt::DisplayRole);
                row << item;

                row << new QStandardItem(rp[4].toString());
                row << new QStandardItem(rp[5].toString());
                row << new QStandardItem(rp[6].toString());

                model->appendRow(row);
            }
        }
        break;
    case eSetting::Fines:
        if (true)
        {
            model->setHorizontalHeaderLabels({ "ID", "Дата", "Машина", "Водитель", "Сумма", "Оплачен", "Описание" });
            QVariantList report;
            ui->checkBox->setText("Не оплаченные");
            if (ui->checkBox->isChecked())
                report = ReportOperations::getNotPaidFinesReport();
            else
                report = ReportOperations::getFinesReport();
            for (QVariant &fine: report)
            {
                QVariantList rp = fine.toList();
                QList<QStandardItem *> row;

                row << new QStandardItem(rp[0].toString());
                row << new QStandardItem();
                row[1]->setData(rp[1].toDate(), Qt::DisplayRole);
                row << new QStandardItem(rp[2].toString());
                row << new QStandardItem(rp[3].toString());
                row << new QStandardItem();
                row[4]->setData(rp[4].toInt(), Qt::DisplayRole);
                row << new QStandardItem(rp[5].toBool() ? "Да" : "Нет");
                row << new QStandardItem(rp[6].toString());

                model->appendRow(row);
            }
        }
        break;
    default:
        break;
    }

    ui->tableView->setModel(model);
}

void EditableReport::setTableSize()
{
    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    switch (this->mode) {
    case eSetting::Repairs:
    case eSetting::Fines:
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 200);
        ui->tableView->setColumnWidth(6, 200);
        break;
    default:
        break;
    }
}

void EditableReport::on_AddButton_clicked()
{
    AddUpdateRepair *w = new AddUpdateRepair(this->mode, -1);
    w->resize(w->minimumSizeHint());
    w->show();

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setTable();
}

void EditableReport::on_EditButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        AddUpdateRepair *w = new AddUpdateRepair(this->mode, id);
        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}

void EditableReport::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void EditableReport::on_DeleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();
        QString text;
        switch (this->mode) {
        case eSetting::Repairs:
            text = "Вы уверены что хотите удалить ремонт машины " + ui->tableView->model()->index(row, 1).data().toString() + "?";
            break;
        case eSetting::Fines:
            text = "Вы уверены что хотите удалить штраф машины " + ui->tableView->model()->index(row, 2).data().toString() + ", водителя " + ui->tableView->model()->index(row, 3).data().toString() + ", на сумму " + ui->tableView->model()->index(row, 4).data().toString() + "?";
            break;
        default:
            break;
        }
        CustomDialog *d = new CustomDialog(text);
        connect(d, &CustomDialog::yesClicked, this, &EditableReport::onYes);
        connect(d, &CustomDialog::noClicked, this, &EditableReport::onNo);
        d->show();
    }
}

void EditableReport::onYes()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->mode)
        {
        case eSetting::Repairs:
            Operations::deleteRepair(id);
            break;
        case eSetting::Fines:
            Operations::deleteFine(id);
            break;
        default:
            break;
        }

        setTable();
    }
}

void EditableReport::onNo()
{
}

void EditableReport::on_checkBox_checkStateChanged(Qt::CheckState state)
{
    setTable();
}

void EditableReport::on_ToPDFButtonSecond_clicked()
{
    // Здесь реализуйте логику кнопки
    QMessageBox::information(this, "Успех", "Кнопка 'PDF по дням' нажата");
}


void EditableReport::on_ToPDFButton_clicked()
{
    QString title;
    QString type;
    switch (this->mode)
    {
        case eSetting::Repairs:
            title = "Ремонты";
            type = ui->checkBox->isChecked() ? "Не завершенные" : "Все";
            break;
        case eSetting::Fines:
            title = "Штрафы";
            type = ui->checkBox->isChecked() ? "Не оплаченные" : "Все";
            break;
        default:
            break;
    }
    PDFmanager::exportToPDF(title, type, { ui->tableView->model() });
}
