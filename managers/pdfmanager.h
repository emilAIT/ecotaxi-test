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
#include "ReportOperations.h"

class ReportOperations;
class PDFmanager
{
public:
    // Конструктор с передачей указателя на объект ReportOperations
    PDFmanager(ReportOperations* reportOps) : reportOperations(reportOps) {}

    // general
    void static exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    void static ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    QString static modelToHTML(QAbstractItemModel *model, int start);

    void static createPDF(QString html, QString title);

    // Методы для генерации отчетов по дням
    QString modelToHTMLByDay();
    void setChargesByDay(const QMap<QDate, QList<QVariantList>> &charges);
    void setEventsByDay(const QMap<QDate, QList<QVariantList>>& events);

private:
    // Статические методы для получения директорий и стилей
    QString static getAppDir();
    QString static getDesktopDir();

    QString static getStyleSheet();

    QString static getHeader(QDateTime time);
    QString static getFooter(QDateTime time);

    // Словари для хранения данных по дням
    QMap<QDate, QList<QVariantList>> eventsByDay;
    QMap<QDate, QList<QVariantList>> chargesByDay;

    // Указатель на объект ReportOperations
    ReportOperations* reportOperations;
};

#endif // PDFMANAGER_H
