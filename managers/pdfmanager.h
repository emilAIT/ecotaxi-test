#ifndef PDFMANAGER_H
#define PDFMANAGER_H

#include <QCoreApplication>
#include <QTextDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>

#include <QPdfWriter>
#include <QPrinter>
#include <QStandardPaths>
#include <QDateTime>
#include <QAbstractItemModel>

#include <QStandardItemModel>
#include <QStandardItem>
#include "operations.h"


class PDFmanager
{
public:
    PDFmanager();

    // Экспорт в PDF
    static void exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    static void ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start, bool separateByDays);

    // Конвертация модели в HTML (учитывая выбранные колонки)
    static QString modelToHTML(QAbstractItemModel *model, int start);

private:
    static QString getAppDir();
    static QString getDesktopDir();
    static QString getStyleSheet();
    static QString getHeader(QDateTime time);
    static QString getFooter(QDateTime time);

    static void createPDF(QString html, QString title);
};

#endif // PDFMANAGER_H
