#ifndef COLUMNSELECTIONBYDAYDIALOG_H
#define COLUMNSELECTIONBYDAYDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QList>
#include <QMessageBox>

#include "../managers/pdfmanager.h"

class ColumnSelectionByDayDialog : public QDialog
{
    Q_OBJECT

public:
    ColumnSelectionByDayDialog(const QList<QAbstractItemModel*> &models, QString title, QString dates, int start, QWidget *parent = nullptr);

    QList<QList<int>> getSelectedColumns() const;

private slots:
    void onExportClicked();
private:
    QList<QAbstractItemModel*> models_;
    QList<QList<QCheckBox*>> checkBoxes_; // 2D list for checkboxes
    QPushButton *exportButton_;
    QString title;
    QString dates;
    int start;
};

#endif // COLUMNSELECTIONBYDAYDIALOG_H
