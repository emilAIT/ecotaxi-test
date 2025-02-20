#include "pdfmanager.h"
#include "dbmanager.h"


PDFmanager::PDFmanager() {}

QString PDFmanager::getStyleSheet()
{
  return
      R"S(
/*
reset css
*/
* {
  -webkit-tap-highlight-color: transparent;
}
*:focus {
  outline: none;
}
html,
body,
div,
span,
applet,
object,
iframe,
h1,
h2,
h3,
h4,
h5,
h6,
p,
blockquote,
pre,
a,
abbr,
acronym,
address,
big,
cite,
code,
del,
dfn,
em,
img,
ins,
kbd,
q,
s,
samp,
small,
strike,
strong,
sub,
sup,
tt,
var,
b,
u,
i,
dl,
dt,
dd,
ol,
ul,
li,
fieldset,
form,
label,
legend,
table,
caption,
tbody,
tfoot,
thead,
tr,
th,
td,
article,
aside,
canvas,
details,
embed,
figure,
figcaption,
footer,
header,
hgroup,
menu,
nav,
output,
ruby,
section,
summary,
time,
mark,
audio,
video {
  margin: 0;
  padding: 0;
  border: 0;
  vertical-align: baseline;
}
article,
aside,
details,
figcaption,
figure,
footer,
header,
hgroup,
menu,
nav,
section {
  display: block;
}
body,
#root {
  line-height: 1;
  overflow-wrap: anywhere;
  overflow-x: hidden;
}
ol,
ul {
  list-style: none;
}
blockquote,
q {
  quotes: none;
}
blockquote:before,
blockquote:after,
q:before,
q:after {
  content: "";
  content: none;
}
table {
  margin: 0;
  border-collapse: collapse;
  border-spacing: 0;
}

table, th, td {
  border: 1px solid;
}

h1, h2, p, td {
  text-align: center;
}

h1 {
  color: #007700;
}
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
  if (!folder.exists())
  {
    folder.mkdir(appDir + "/отчеты");
  }

  QString fileName = title + " " + time.toString("dd.MM.yyyy HH-mm-ss") + ".pdf";
  fileName.replace(" ", "_");

  QString filePath = appDir + "/отчеты/" + fileName;

  QPrinter printer(QPrinter::PrinterResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPageSize(QPageSize::A4);
  printer.setOutputFileName(filePath);

  qDebug() << printer.outputFileName();

  QTextDocument doc;

  doc.setDefaultStyleSheet(getStyleSheet());
  doc.setHtml(getHeader(time) + html + getFooter(time));
  doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());

  doc.print(&printer);

  QMimeData *mimeData = new QMimeData();
  mimeData->setUrls({QUrl::fromLocalFile(filePath)});

  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setMimeData(mimeData);
  
  QApplication::restoreOverrideCursor();

  QMessageBox popup;

  popup.setTextFormat(Qt::MarkdownText);
  popup.setText("Отчет сохранен в папке отчеты на рабочем столе и скопирован в буфер обмена");

  popup.exec();
}

QString PDFmanager::getHeader(QDateTime time)
{
  return "<p>" + time.toString("dd.MM.yyyy HH:mm:ss") + "</p><h1 width=100% color='#007700'>ECO TAXI</h1>";
}

QString PDFmanager::getFooter(QDateTime time)
{
  return "<br><p>ECO TAXI</p><p>" + time.toString("dd.MM.yyyy HH:mm:ss") + "</p>";
}

void PDFmanager::ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
{
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + dates + "</p><br>";

  for (int i = 0; i < models.size(); i++)
  {
    html += modelToHTML(models[i], start != 0 && i == 0 ? 1 : 0);
  }

  createPDF(html, title + " " + dates);
}

QString PDFmanager::modelToHTML(QAbstractItemModel *model, int start)
{
    QString html;
    
    // Remove margin, and set table width to 100%
    html += "<table style='margin: 0;' margin=0 width=100%><tr>";
    
    // Add row number column if start == 1
    if (start == 1)
    {
        html += "<th>#</th>";
    }
    
    // Add headers
    for (int i = start; i < model->columnCount(); i++)
    {
        html += "<th>" + model->headerData(i, Qt::Horizontal).toString() + "</th>";
    }
    html += "</tr>";
    
    // Add rows
    for (int i = 0; i < model->rowCount(); i++)
    {
        html += "<tr>";
        if (start == 1)
        {
            html += "<td>" + QString::number(i + 1) + "</td>";
        }
        
        for (int j = start; j < model->columnCount(); j++)
        {
            QString cellData = model->index(i, j).data(Qt::DisplayRole).toString();
            QString header = model->headerData(j, Qt::Horizontal).toString();
            
            // Check if the header is "Инвестору" to apply green color
            if (header == "Инвестору" && start != 1)
            {
                html += "<td style='border: 1px solid black; color:#007700;'>" + cellData + "</td>";
            }
            else
            {
                html += "<td>" + cellData + "</td>";
            }
        }
        html += "</tr>";
    }
    
    html += "</table>";
    return html;
}

void PDFmanager::exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
{
    ColumnSelectionDialog dialog(models, title, dates, start);
    
    dialog.exec();
}






void PDFmanager::createPDFByDay(QString html, QString title)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDateTime time = QDateTime::currentDateTime();
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/отчеты по дням");
    if (!folder.exists())
    {
        folder.mkdir(appDir + "/отчеты по дням");
    }

    QString fileName = title + " " + time.toString("dd.MM.yyyy HH-mm-ss") + ".pdf";
    fileName.replace(" ", "_");

    QString filePath = appDir + "/отчеты по дням/" + fileName; // исправлен путь

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName(filePath);

    qDebug() << printer.outputFileName();

    QTextDocument doc;

    doc.setDefaultStyleSheet(getStyleSheet());
    doc.setHtml(getHeader(time) + html + getFooter(time)); // Используем заголовок и подвал по дням
    doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());

    doc.print(&printer);

    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls({QUrl::fromLocalFile(filePath)});

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);

    QApplication::restoreOverrideCursor();

    QMessageBox popup;

    popup.setTextFormat(Qt::MarkdownText);
    popup.setText("Отчет сохранен в папке \"отчеты по дням\" на рабочем столе и скопирован в буфер обмена");

    popup.exec();
}

void PDFmanager::exportToPDFByDay(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
{
    ColumnSelectionByDayDialog dialog(models, title, dates, start);

    dialog.exec();
}

void PDFmanager::ToPDFByDay(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
{
    QDate fromDate = QDate::fromString(dates.left(10), "dd.MM.yyyy");
    QDate toDate = QDate::fromString(dates.right(10), "dd.MM.yyyy");

    fetchDataByDay(fromDate, toDate); // Загружаем события и зарядки за день

    QString html = "<h2>" + title + "</h2>";
    html += "<p>" + dates + "</p><br>";

    html += modelToHTMLByDay();

    createPDFByDay(html, title + " " + dates);
}

void PDFmanager::fetchDataByDay(QDate fromDate, QDate toDate)
{
    eventsByDay.clear();
    chargesByDay.clear();

    QSqlQuery query;

    // Запрос событий
    query.prepare(R"(
        SELECT DATE(e.date) as event_date, e.id,
               c.sid as car_name, d.name as driver_name,
               t.name as event_type, e.amount, e.description
        FROM events e
        JOIN cars c ON e.carId = c.id
        JOIN drivers d ON e.driverId = d.id
        JOIN types t ON e.typeId = t.id
        WHERE DATE(e.date) BETWEEN :fromDate AND :toDate
        ORDER BY event_date;
    )");
    query.bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
    query.bindValue(":toDate", toDate.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Ошибка запроса событий:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QVariantList eventData;
        eventData.append(query.value("id"));
        eventData.append(query.value("car_name"));
        eventData.append(query.value("driver_name"));
        eventData.append(query.value("event_type"));
        eventData.append(query.value("amount"));
        eventData.append(query.value("description"));

        QDate eventDate = query.value("event_date").toDate();
        eventsByDay[eventDate].append(eventData);
    }

    // Запрос зарядок
    query.prepare(R"(
        SELECT DATE(c.date) as charge_date, c.id,
               cars.sid as car_name, drivers.name as driver_name,
               locations.name as location_name, c.kwh, c.duration
        FROM charges c
        JOIN cars ON c.carId = cars.id
        JOIN drivers ON c.driverId = drivers.id
        JOIN locations ON c.locationId = locations.id
        WHERE DATE(c.date) BETWEEN :fromDate AND :toDate
        ORDER BY charge_date;
    )");
    query.bindValue(":fromDate", fromDate.toString("yyyy-MM-dd"));
    query.bindValue(":toDate", toDate.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Ошибка запроса зарядок:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QVariantList chargeData;
        chargeData.append(query.value("id"));
        chargeData.append(query.value("car_name"));
        chargeData.append(query.value("driver_name"));
        chargeData.append(query.value("location_name"));
        chargeData.append(query.value("kwh"));
        chargeData.append(query.value("duration"));

        QDate chargeDate = query.value("charge_date").toDate();
        chargesByDay[chargeDate].append(chargeData);
    }
}

QString PDFmanager::modelToHTMLByDay()
{
    QString html;
    bool firstPage = true;

    for (const QDate &date : eventsByDay.keys()) {
        if (!firstPage) {
            html += "<div style='page-break-before: always;'></div>"; // Разрыв страницы
        }
        firstPage = false;

        html += "<h2>" + date.toString("dd.MM.yyyy") + "</h2>";

        // События
        html += "<h3>События</h3>";
        html += "<table border='1' width='100%'><tr><th>ID</th><th>Машина</th><th>Водитель</th><th>Тип</th><th>Сумма</th><th>Описание</th></tr>";
        for (const QVariantList &event : eventsByDay[date]) {
            html += "<tr>";
            for (const QVariant &value : event) {
                html += "<td>" + value.toString() + "</td>";
            }
            html += "</tr>";
        }
        html += "</table><br>";

        // Зарядки
        if (chargesByDay.contains(date)) {
            html += "<h3>Зарядки</h3>";
            html += "<table border='1' width='100%'><tr><th>ID</th><th>Машина</th><th>Водитель</th><th>Локация</th><th>КВт</th><th>Время</th></tr>";
            for (const QVariantList &charge : chargesByDay[date]) {
                html += "<tr>";
                for (const QVariant &value : charge) {
                    html += "<td>" + value.toString() + "</td>";
                }
                html += "</tr>";
            }
            html += "</table><br>";
        }
    }
    return html;
}

QMap<QDate, QList<QVariantList>> PDFmanager::eventsByDay;
QMap<QDate, QList<QVariantList>> PDFmanager::chargesByDay;
