#include "pdfmanager.h"
#include "usersession.h"
#include <QDir>
#include <QPrinter>
#include <QTextDocument>
#include <QMimeData>
#include <QClipboard>
#include <QMessageBox>
#include <QStandardPaths>
#include <QApplication>

PDFmanager::PDFmanager() {}

QString PDFmanager::getStyleSheet()
{
    return R"S(
        * { -webkit-tap-highlight-color: transparent; }
        *:focus { outline: none; }
        html, body, div, span, applet, object, iframe, h1, h2, h3, h4, h5, h6, p, blockquote, pre, a, abbr, acronym, address, big, cite, code, del, dfn, em, img, ins, kbd, q, s, samp, small, strike, strong, sub, sup, tt, var, b, u, i, dl, dt, dd, ol, ul, li, fieldset, form, label, legend, table, caption, tbody, tfoot, thead, tr, th, td, article, aside, canvas, details, embed, figure, figcaption, footer, header, hgroup, menu, nav, output, ruby, section, summary, time, mark, audio, video {
            margin: 0; padding: 0; border: 0; vertical-align: baseline;
        }
        article, aside, details, figcaption, figure, footer, header, hgroup, menu, nav, section { display: block; }
        body, #root { line-height: 1; overflow-wrap: anywhere; overflow-x: hidden; }
        ol, ul { list-style: none; }
        blockquote, q { quotes: none; }
        blockquote:before, blockquote:after, q:before, q:after { content: ""; content: none; }
        table { margin: 0; border-collapse: collapse; border-spacing: 0; }
        table, th, td { border: 1px solid; }
        h1, h2, p, td { text-align: center; }
        h1 { color: #007700; }
    )S";
}

QString PDFmanager::getAppDir()
{
    return QCoreApplication::applicationDirPath();
}

QString PDFmanager::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void PDFmanager::createPDF(QString html, QString title)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDateTime time = QDateTime::currentDateTime();
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/отчеты");
    if (!folder.exists()) {
        folder.mkdir(appDir + "/отчеты");
    }

    QString fileName = title + " " + time.toString("dd.MM.yyyy HH-mm-ss") + ".pdf";
    fileName.replace(" ", "_");

    QString filePath = appDir + "/отчеты/" + fileName;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName(filePath);

    QTextDocument doc;
    doc.setDefaultStyleSheet(getStyleSheet());
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());
    doc.print(&printer);

    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls({QUrl::fromLocalFile(filePath)});

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);

    QApplication::restoreOverrideCursor();

    QMessageBox::information(nullptr, "Успех", "Отчет сохранен в папке отчеты на рабочем столе и скопирован в буфер обмена");
}

QString PDFmanager::getHeader(QDateTime time)
{
    return "<p>" + time.toString("dd.MM.yyyy HH:mm:ss") + "</p><h1 width=100% color='#007700'>ECO TAXI</h1>";
}

QString PDFmanager::getFooter(QDateTime time)
{
    return "<br><p>ECO TAXI</p><p>" + time.toString("dd.MM.yyyy HH:mm:ss") + "</p>";
}

void PDFmanager::ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start, bool exportByDays)
{
    QString html;
    QDateTime startDate, endDate;


    // Парсим диапазон дат
    if (dates.contains("-")) {
        QStringList dateRange = dates.split("-");
        startDate = QDateTime::fromString(dateRange[0].trimmed(), "dd.MM.yyyy");
        endDate = QDateTime::fromString(dateRange[1].trimmed(), "dd.MM.yyyy");
    } else {
        startDate = QDateTime::fromString(dates, "dd.MM.yyyy");
        endDate = startDate;
    }

    if (exportByDays) {
        QDateTime dateTime = QDateTime::fromString(dates, "dd.MM.yyyy");
        QDateTime currentDate = startDate;
        while (currentDate <= endDate) {
            QString dateStr = currentDate.toString("dd.MM.yyyy");
            QDate date = QDate::fromString(dateStr, "dd.MM.yyyy"); // Преобразуем в QDate

            // Создаем модель для текущей даты
            QStandardItemModel* model = new QStandardItemModel();
            static const QStringList HEADERS = {"-", "Время", "Тип", "ID Водителя",
                                                "ID Машины", "Сумма", "Описание"};
            model->setHorizontalHeaderLabels(HEADERS);

            // Получаем события из БД для текущей даты
            QVariantList events = Operations::selectEventsByDate(date); // Передаем QDate

            // Заполняем модель
            for (const QVariant& event : events) {
                QList<QStandardItem*> row;
                QVariantList data = event.toList();

                row.append(new QStandardItem(data[0].toString())); // id

                QStandardItem* dateItem = new QStandardItem();
                QDateTime dateTime = data[1].toDateTime();
                dateItem->setData(dateTime.toString("HH:mm:ss"), Qt::DisplayRole); // Только время
                row.append(dateItem);

                row.append(new QStandardItem(data[2].toString())); // Тип
                row.append(new QStandardItem(data[3].toString())); // ID Водителя
                row.append(new QStandardItem(data[4].toString())); // ID Машины

                QStandardItem* amountItem = new QStandardItem();
                amountItem->setData(data[5].toInt(), Qt::DisplayRole); // Сумма
                row.append(amountItem);

                row.append(new QStandardItem(data[6].toString())); // Описание
                model->appendRow(row);
            }

            // Сортировка по времени (1-й столбец)
            model->sort(1, Qt::AscendingOrder);

            // Генерируем HTML для текущей даты
            html += getHeader(dateTime);
            html += "<h2>" + dateStr + "</h2>";

            if (model->rowCount() > 0) {
                html += modelToHTML(model, start);
            } else {
                html += "<p>Нету записей на эту дату :(</p>";
            }

            html += getFooter(dateTime);
            if (currentDate < endDate) {
                html += "<div style='page-break-before: always;'></div>";
            }

            delete model;
            currentDate = currentDate.addDays(1);
        }
    } else {
        // Оригинальная логика для экспорта без разделения по дням
        QDateTime dateTime = QDateTime::fromString(dates, "dd.MM.yyyy");
        html += getHeader(dateTime);
        html += "<h2>" + title + "</h2>";
        html += "<p>" + dates + "</p><br>";

        for (int i = 0; i < models.size(); i++) {
            html += modelToHTML(models[i], start);
        }
        html += getFooter(dateTime);
    }

    createPDF(html, title + " " + dates);
}

QString PDFmanager::modelToHTML(QAbstractItemModel *model, int start)
{
    QString html;

    // Начало таблицы
    html += "<div style='width: 100%; margin: 20px 0;'>";
    html += "<table style='width: 100%; border-collapse: collapse; margin: 0 auto;'>";

    // Заголовок таблицы
    html += "<thead><tr>";
    if (start == 1) {
        html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>#</th>";
    }
    for (int i = start; i < model->columnCount(); i++) {
        html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>"
                + model->headerData(i, Qt::Horizontal).toString()
                + "</th>";
    }
    html += "</tr></thead><tbody>";

    // Данные таблицы
    for (int i = 0; i < model->rowCount(); i++) {
        html += "<tr>";
        if (start == 1) {
            html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>"
                    + QString::number(i + 1)
                    + "</td>";
        }
        for (int j = start; j < model->columnCount(); j++) {
            QString cellData = model->index(i, j).data(Qt::DisplayRole).toString();
            QString header = model->headerData(j, Qt::Horizontal).toString();
            QString cellStyle = "border: 1px solid black; padding: 5px; text-align: center;";
            if (header == "Инвестору" && start != 1) {
                cellStyle += " color: #007700;";
            }
            html += "<td style='" + cellStyle + "'>" + cellData + "</td>";
        }
        html += "</tr>";
    }

    html += "</tbody></table></div>";
    return html;
}

void PDFmanager::exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
{
    // Диалог для выбора типа экспорта
    QMessageBox dialog;
    dialog.setWindowTitle("Тип экспорта");
    dialog.setText("Выберите тип экспорта:");
    userSession &u = userSession::getInstance();
    if (u.checkIsAdmin())
        dialog.addButton("Экспорт по дням", QMessageBox::YesRole);
    dialog.addButton("Общий экспорт", QMessageBox::NoRole);

    dialog.exec();

    // Определяем, какой тип экспорта выбран
    bool exportByDays = (dialog.buttonRole(dialog.clickedButton()) == QMessageBox::YesRole);

    // Вызываем ToPDF с выбранным типом экспорта
    ToPDF(title, dates, models, start, exportByDays);
}
