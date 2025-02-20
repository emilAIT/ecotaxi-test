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
#include "../scripts/columnselectionbydaydialog.h"

class PDFmanager
{
public:
    PDFmanager();

    // general
    void static exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    void static ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    QString static modelToHTML(QAbstractItemModel *model, int start);


    ///////////////
    void static ToPDFByDay(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    void static exportToPDFByDay(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    void static fetchDataByDay(QDate fromDate, QDate toDate);

    QString static modelToHTMLByDay();

//////////////

private:
    QString static getAppDir();
    QString static getDesktopDir();

    QString static getStyleSheet();

    QString static getHeader(QDateTime time);
    QString static getFooter(QDateTime time);

    void static createPDF(QString html, QString title);



    static QMap<QDate, QList<QVariantList>> eventsByDay;  // для событий
    static QMap<QDate, QList<QVariantList>> chargesByDay; // для зарядок

    void static createPDFByDay(QString html, QString title);


};

#endif // PDFMANAGER_H
