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

#include "../managers/operations.h"
#include "../managers/reportoperations.h"

class PDFmanager
{
public:
    PDFmanager();

    // general reports
    void static createCarsReport(QDate from, QDate to);

    void static createDriversReport(QDate from, QDate to);

    void static createInvestorsReport(QDate from, QDate to);

    void static createTypesReport(QDate from, QDate to);

    void static createLocationsReport(QDate from, QDate to);

    void static createChargesReport(QDate from, QDate to);

    void static createUsersReport(QDate from, QDate to);

    // reports by item
    void static createCarReport(QDate from, QDate to, int id);

    void static createDriverReport(QDate from, QDate to, int id);

    void static createInvestorReport(QDate from, QDate to, int id);

    void static createTypeReport(QDate from, QDate to, int id);

    void static createLocationReport(QDate from, QDate to, int id);

    void static createChargeReport(QDate from, QDate to, int id);

    void static createUserReport(QDate from, QDate to, int id);

private:
    QString static getAppDir();
    QString static getDesktopDir();

    QString static getStyleSheet();

    QString static getHeader(QDateTime time);
    QString static getFooter(QDateTime time);

    void static createPDF(QString html, QString title);
};

#endif // PDFMANAGER_H
