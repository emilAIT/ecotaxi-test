#include "ColumnSelectionDialog.h"
#include <QStandardItemModel>
#include <QStandardItem>

ColumnSelectionDialog::ColumnSelectionDialog(const QList<QAbstractItemModel*> &models, QString title, QString dates, int start, QWidget *parent)
    : QDialog(parent), models_(models), title(title), dates(dates), start(start)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Добавляем комбо-бокс для выбора типа отчета
    comboBoxExport_ = new QComboBox(this);
    comboBoxExport_->addItem("Общий отчет");
    comboBoxExport_->addItem("Отчет по дням");
    layout->addWidget(comboBoxExport_);

    exportButton_ = new QPushButton("Экспорт", this);
    connect(exportButton_, &QPushButton::clicked, this, &ColumnSelectionDialog::onExportClicked);
    layout->addWidget(exportButton_);

    setLayout(layout);
    setWindowTitle("Выбор столбцов для экспорта");
}


bool ColumnSelectionDialog::isSeparatedByDays() const
{
    // Если выбран "Отчет по дням", возвращаем true
    return comboBoxExport_->currentIndex() == 1;
}

void ColumnSelectionDialog::onExportClicked()
{
    // Получаем выбранный тип отчета
    bool separateByDays = isSeparatedByDays();

    // Передаем данные в PDFmanager
    PDFmanager::ToPDF(this->title, this->dates, models_, start, separateByDays);

    accept();
}
