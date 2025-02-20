#include "ColumnSelectionByDayDialog.h"

ColumnSelectionByDayDialog::ColumnSelectionByDayDialog(const QList<QAbstractItemModel*> &models, QString title, QString dates, int start, QWidget *parent)
    : QDialog(parent), models_(models)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->title = title;
    this->dates = dates;
    this->start = start;

    for (int j = 0; j < models_.size(); ++j) {
        QHBoxLayout *modelLayout = new QHBoxLayout();
        QList<QCheckBox*> modelCheckBoxes;

        for (int i = 0; i < models_[j]->columnCount(); ++i) {
            QCheckBox *checkBox = new QCheckBox(models_[j]->headerData(i, Qt::Horizontal).toString());
            checkBox->setChecked(true);

            if (i == 0 && (start != 0 || j != 0)) {
                checkBox->hide();
            }

            modelLayout->addWidget(checkBox);
            modelCheckBoxes.append(checkBox);
        }

        checkBoxes_.append(modelCheckBoxes);
        layout->addLayout(modelLayout);
    }

    exportButton_ = new QPushButton("Экспорт", this);
    connect(exportButton_, &QPushButton::clicked, this, &ColumnSelectionByDayDialog::onExportClicked);
    layout->addWidget(exportButton_);

    setLayout(layout);
    setWindowTitle("Выбор столбцов для экспорта по дням");
}

QList<QList<int>> ColumnSelectionByDayDialog::getSelectedColumns() const
{
    QList<QList<int>> selectedColumns;
    for (int i = 0; i < models_.size(); ++i) {
        QList<int> selected;
        for (int j = 0; j < models_[i]->columnCount(); ++j) {
            if (checkBoxes_[i][j]->isChecked()) {
                selected.append(j);
            }
        }
        selectedColumns.append(selected);
    }
    return selectedColumns;
}

void ColumnSelectionByDayDialog::onExportClicked()
{
    QList<QList<int>> selectedColumns = getSelectedColumns();

    if (selectedColumns.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите хотя бы один столбец для экспорта.");
        return;
    }

    QList<QAbstractItemModel *> selectedModels;
    for (int i = 0; i < models_.size(); ++i) {
        QStandardItemModel *filteredModel = new QStandardItemModel();
        const QList<int> &selected = selectedColumns[i];

        for (int columnIndex = 0; columnIndex < selected.size(); ++columnIndex) {
            int originalIndex = selected[columnIndex];
            filteredModel->setHorizontalHeaderItem(columnIndex, new QStandardItem(models_[i]->headerData(originalIndex, Qt::Horizontal).toString()));
        }

        for (int row = 0; row < models_[i]->rowCount(); ++row) {
            for (int columnIndex = 0; columnIndex < selected.size(); ++columnIndex) {
                int originalIndex = selected[columnIndex];
                filteredModel->setItem(row, columnIndex, new QStandardItem(models_[i]->index(row, originalIndex).data(Qt::DisplayRole).toString()));
            }
        }
        selectedModels.append(filteredModel);
    }

    PDFmanager::ToPDFByDay(this->title, this->dates, selectedModels, start);

    accept();
}
