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

#include <QDate>

#include "qstandarditemmodel.h"
#include "../scripts/columnselectiondialog.h"

class PDFmanager
{
public:
    PDFmanager();

    // general
    void static exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);
    void static pdfmanagerbydays(QString title, QString dates, QList<QAbstractItemModel*> models);

    void static ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    QString static modelToHTML(QAbstractItemModel *model, int start);

    void createCarReport(const QString& startDate, const QString& endDate, QList<QAbstractItemModel*> dailyModels);
private:
    void insertPageBreak(QTextDocument& doc);
    // Make formatDateHeader static if it doesn't depend on instance data
    static QString formatDateHeader(const QDateTime& date);

    QString static getAppDir();
    QString static getDesktopDir();

    QString static getStyleSheet();
    // Make it static
    static QString createDailyReportPage(const QDateTime& date, QAbstractItemModel* model);

    QString static getHeader(QDateTime time);
    QString static getFooter(QDateTime time);

    void static createPDF(QString html, QString title);

    void static printMultiplePages(QString title,
                                   QString dates,
                                   QMap<QString, QAbstractItemModel*> dateToModel,
                                   int start);
};

#endif // PDFMANAGER_H
