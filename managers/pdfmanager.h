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

    void static ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    void static createDailyPDF(QString title, QString dates, const QList<QVariantList> &dailyReports);

    void static createPDF(QString html, QString title);

    QString static modelToHTML(QAbstractItemModel *model, int start);

    void createPDFByDay(QString html, QString title);

    void exportToPDFByDay(QString title, QString dates, QList<QAbstractItemModel *> models, int start);

    void fetchDataByDay(QDate fromDate, QDate toDate);

    QString modelToHTMLByDay();


private:
    QString static getAppDir();
    QString static getDesktopDir();

    QString static getStyleSheet();

    QString static getHeader(QDateTime time);
    QString static getFooter(QDateTime time);

    static QMap<QDate, QList<QVariantList>> eventsByDay;
    static QMap<QDate, QList<QVariantList>> chargesByDay;

};

#endif // PDFMANAGER_H
