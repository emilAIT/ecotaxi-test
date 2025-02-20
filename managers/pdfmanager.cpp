#include "pdfmanager.h"
#include "reportoperations.h" // Путь может отличаться - используйте правильный путь
#include "operations.h"       // Подключите заголовочный файл Operations
#include <QStandardItemModel>

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
  html += "<div style='width: 100%; margin: 20px 0;'>";
  html += "<table style='width: 100%; border-collapse: collapse; margin: 0 auto;'>";

  // Добавляем заголовки
  html += "<thead><tr>";
  if (start == 1)
  {
    html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>#</th>";
  }

  // Специальная обработка для отчета по зарядкам водителей
  if (model->headerData(0, Qt::Horizontal).toString() == "Водитель")
  {
    html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>Водитель</th>";
    html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>KWH</th>";
  }
  else
  {
    // Стандартная обработка для других типов отчетов
    for (int i = start; i < model->columnCount(); i++)
    {
      html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>" + model->headerData(i, Qt::Horizontal).toString() + "</th>";
    }
  }
  html += "</tr></thead><tbody>";
  // Добавляем строки
  for (int i = 0; i < model->rowCount(); i++)
  {
    html += "<tr>";
    if (start == 1)
    {
      html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>" + QString::number(i + 1) + "</td>";
    }

    // Проверяем, нужна ли специальная обработка для отчета по зарядкам водителей
    if (model->headerData(0, Qt::Horizontal).toString() == "Водитель")
    {
      // Водитель
      QVariant driverNameData = model->index(i, 0).data(Qt::DisplayRole);
      html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>" + driverNameData.toString() + "</td>";

      // KWH
      QVariant kwhData = model->index(i, 1).data(Qt::DisplayRole);
      html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>" + kwhData.toString() + "</td>";
    }
    else
    {
      // Стандартная обработка для других типов отчетов
      for (int j = start; j < model->columnCount(); j++)
      {
        QVariant cellData = model->index(i, j).data(Qt::DisplayRole);
        QString cellText = cellData.toString();

        QString cellStyle = "border: 1px solid black; padding: 5px; text-align: center;";
        html += "<td style='" + cellStyle + "'>" + cellText + "</td>";
      }
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

QString fileName = title + "_" + fromDate.toString("dd.MM.yyyy") + "_-_" +
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

// Для каждого дня запрашиваем отдельные данные
QDate currentDate = fromDate;
while (currentDate <= toDate)
{
if (currentDate != fromDate)
{
html += "<div style='page-break-before: always;'></div>";
}

html += "<h2>Отчет по машинам за " + currentDate.toString("dd.MM.yyyy") + "</h2><br>";

// Получаем данные за текущий день
QVariantList dailyCarsData = ReportOperations::getCarsReport(currentDate, currentDate);

if (dailyCarsData.isEmpty()) {
html += "<p style='text-align: center; margin: 20px 0;'>Нет данных за этот день</p>";
} else {
// Создаем временную модель данных для текущего дня
QStandardItemModel dailyModel;

// Копируем заголовки из оригинальной модели
for (int col = 0; col < models[0]->columnCount(); col++) {
QString headerText = models[0]->headerData(col, Qt::Horizontal).toString();
dailyModel.setHorizontalHeaderItem(col, new QStandardItem(headerText));
}

// Заполняем модель данными за текущий день
for (const QVariant &rowData : dailyCarsData) {
QList<QStandardItem*> items;
QVariantList dataList = rowData.toList();

for (const QVariant &cellData : dataList) {
QStandardItem *item = new QStandardItem();
if (cellData.type() == QVariant::Int || cellData.type() == QVariant::Double) {
item->setData(cellData, Qt::DisplayRole);
} else {
item->setText(cellData.toString());
}
items << item;
}

dailyModel.appendRow(items);
}


// Преобразуем модели в HTML
html += modelToHTML(&dailyModel, start);
}

currentDate = currentDate.addDays(1);
}

doc.setHtml(getHeader(currentTime) + html + getFooter(currentTime));
doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());

doc.print(&printer);

QMimeData *mimeData = new QMimeData();
mimeData->setUrls({QUrl::fromLocalFile(filePath)});
QClipboard *clipboard = QApplication::clipboard();
clipboard->setMimeData(mimeData);

QApplication::restoreOverrideCursor();

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
