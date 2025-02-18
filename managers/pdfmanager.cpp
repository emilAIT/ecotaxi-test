#include "pdfmanager.h"
#include "reportoperations.h" // Путь может отличаться - используйте правильный путь

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

    // Start with a div container for better PDF rendering
    html += "<div style='width: 100%; margin: 20px 0;'>";

    // Add table with explicit styling for PDF rendering
    html += "<table style='width: 100%; border-collapse: collapse; margin: 0 auto;'>";

    // Add header row
    html += "<thead><tr>";

    // Add row number column if start == 1
    if (start == 1)
    {
        html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>#</th>";
    }

    // Add headers
    for (int i = start; i < model->columnCount(); i++)
    {
        html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>"
                + model->headerData(i, Qt::Horizontal).toString()
                + "</th>";
    }
    html += "</tr></thead><tbody>";

    // Add rows
    for (int i = 0; i < model->rowCount(); i++)
    {
        html += "<tr>";
        if (start == 1)
        {
            html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>"
                    + QString::number(i + 1)
                    + "</td>";
        }

        for (int j = start; j < model->columnCount(); j++)
        {
            QString cellData = model->index(i, j).data(Qt::DisplayRole).toString();
            QString header = model->headerData(j, Qt::Horizontal).toString();

            // Check if the header is "Инвестору" to apply green color
            QString cellStyle = "border: 1px solid black; padding: 5px; text-align: center;";
            if (header == "Инвестору" && start != 1)
            {
                cellStyle += " color: #007700;";
            }

            html += "<td style='" + cellStyle + "'>" + cellData + "</td>";
        }
        html += "</tr>";
    }

    html += "</tbody></table></div>";
    return html;
}


void PDFmanager::exportToPDFByDates(QString title, QDate fromDate, QDate toDate, 
  QList<QAbstractItemModel *> models, int start)
{
QApplication::setOverrideCursor(Qt::WaitCursor);

QDateTime currentTime = QDateTime::currentDateTime();
QString appDir = getDesktopDir();
QDir folder(appDir + "/отчеты");
if (!folder.exists())
{
folder.mkdir(appDir + "/отчеты");
}

QString fileName = title + "_по_дням_" + 
fromDate.toString("dd.MM.yyyy") + "_-_" + 
toDate.toString("dd.MM.yyyy") + "_" + 
currentTime.toString("HH-mm-ss") + ".pdf";
fileName.replace(" ", "_");

QString filePath = appDir + "/отчеты/" + fileName;

QPrinter printer(QPrinter::PrinterResolution);
printer.setOutputFormat(QPrinter::PdfFormat);
printer.setPageSize(QPageSize::A4);
printer.setOutputFileName(filePath);

QTextDocument doc;
doc.setDefaultStyleSheet(getStyleSheet());

QString html = "<h2>" + title + "</h2>";
html += "<p>Период: " + fromDate.toString("dd.MM.yyyy") + " - " + toDate.toString("dd.MM.yyyy") + "</p><br>";

// Получаем класс ReportOperations для доступа к данным
ReportOperations reportOps;

// Для каждого дня в выбранном диапазоне
QDate currentDate = fromDate;
while (currentDate <= toDate)
{
// Добавляем заголовок страницы с текущей датой
if (currentDate != fromDate) {
html += "<div style='page-break-before: always;'></div>";
}

html += "<h2>Отчет по машинам за " + currentDate.toString("dd.MM.yyyy") + "</h2><br>";

// Получаем данные за текущий день
QVariantList dailyData = ReportOperations::getCarsReport(currentDate, currentDate);

if (dailyData.isEmpty())
{
// Если данных нет, показываем сообщение
html += "<p style='text-align: center; font-size: 14pt; margin: 40px 0;'>Нет данных за этот день</p>";
}
else
{
// Формируем HTML-таблицу с данными
html += "<table style='width: 100%; border-collapse: collapse;'>";

// Заголовок таблицы
html += "<thead><tr>";
if (start == 0) {
html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>#</th>";
}

// Добавляем заголовки столбцов
QStringList headers = {"ID", "Инвестор", "Доход", "Налог 10%", "KWH x 10", 
"Расход", "Общий", "Дней", ">0", "Средняя", "%", "Комиссия", "Инвестору"};

for (const QString &header : headers) {
html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>" + 
header + "</th>";
}
html += "</tr></thead><tbody>";

// Добавляем данные
int rowNumber = 1;
for (const QVariant &item : dailyData)
{
QVariantList rowData = item.toList();
html += "<tr>";

// Номер строки
if (start == 0) {
html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>" + 
QString::number(rowNumber++) + "</td>";
}

// Данные ячеек
for (int i = 0; i < rowData.size(); i++) {
QString cellStyle = "border: 1px solid black; padding: 5px; text-align: center;";
if (headers[i] == "Инвестору") {
cellStyle += " color: #007700;";
}

html += "<td style='" + cellStyle + "'>" + rowData[i].toString() + "</td>";
}

html += "</tr>";
}

html += "</tbody></table>";

// Добавляем итоговую таблицу
QVariantList dailySummary = ReportOperations::getAllCarsReport(currentDate, currentDate);
if (!dailySummary.isEmpty()) {
html += "<br><table style='width: 100%; border-collapse: collapse;'>";
html += "<thead><tr>";

QStringList summaryHeaders = {"Итого", "Доход", "Налог 10%", "KWH * 10", 
         "Расход", "Общая", "Комиссия", "Инвесторам"};

for (const QString &header : summaryHeaders) {
html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>" + 
header + "</th>";
}

html += "</tr></thead><tbody><tr>";

// Первая ячейка - "Итого"
html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>Итого</td>";

// Добавляем остальные ячейки с данными
QVariantList summaryData = dailySummary.toList();
for (int i = 0; i < summaryData.size(); i++) {
html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>" + 
summaryData[i].toString() + "</td>";
}

html += "</tr></tbody></table>";
}
}

// Переходим к следующему дню
currentDate = currentDate.addDays(1);
}

// Завершаем документ
doc.setHtml(getHeader(currentTime) + html + getFooter(currentTime));
doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());

// Печатаем документ
doc.print(&printer);

// Копируем путь к файлу в буфер обмена
QMimeData *mimeData = new QMimeData();
mimeData->setUrls({QUrl::fromLocalFile(filePath)});
QClipboard *clipboard = QApplication::clipboard();
clipboard->setMimeData(mimeData);

QApplication::restoreOverrideCursor();

// Показываем сообщение об успешном создании файла
QMessageBox popup;
popup.setTextFormat(Qt::MarkdownText);
popup.setText("Отчет с разбивкой по дням сохранен в папке отчеты на рабочем столе и скопирован в буфер обмена");
popup.exec();
}

void PDFmanager::exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
{
    ColumnSelectionDialog dialog(models, title, dates, start);
    
    dialog.exec();
}
