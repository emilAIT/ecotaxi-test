#ifndef DAILYPDFMANAGER_H
#define DAILYPDFMANAGER_H

#include <QDate>
#include <QAbstractItemModel>
#include <QPrinter>
#include <QTextDocument>
#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QClipboard>
#include <QMimeData>
#include <QMessageBox>
#include "pdfmanager.h"

class DailyPDFManager : public PDFmanager {
public:
    DailyPDFManager(QDate from, QDate to, QAbstractItemModel* model);
    void createDailyPDF();

private:
    QDate m_from;
    QDate m_to;
    QAbstractItemModel* m_model;
    QString getDailyHeader(QDate date);
};

#endif // DAILYPDFMANAGER_H
