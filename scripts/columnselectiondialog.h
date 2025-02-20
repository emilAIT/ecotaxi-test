#ifndef COLUMNSELECTIONDIALOG_H
#define COLUMNSELECTIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QList>
#include <QVBoxLayout>
#include <QLayout>
#include "../managers/pdfmanager.h"

class ColumnSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    ColumnSelectionDialog(const QList<QAbstractItemModel*> &models,
                          QString title,
                          QString dates,
                          int start,
                          QWidget *parent = nullptr);
    bool isSeparatedByDays() const;

private slots:
    void onExportClicked();

private:
    QList<QAbstractItemModel*> models_;
    QPushButton *exportButton_;
    QComboBox *comboBoxExport_;
    QString title;
    QString dates;
    int start;
};

#endif // COLUMNSELECTIONDIALOG_H
