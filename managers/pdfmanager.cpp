#include "pdfmanager.h"

PDFmanager::PDFmanager() {}

QString PDFmanager::getStyleSheet()
{
  return
      R"S(
/*
reset css
*/
* {
  text-decoration: none;
  user-select: none;
  transition: 0.1s;
  -webkit-tap-highlight-color: transparent;
  scroll-margin-top: 50px;
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

void PDFmanager::createCarsReport(QDate from, QDate to)
{
  QString title = "Отчет по машинам";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>ID</th><th>Инвестор</th><th>Доход</th><th>Расход</th><th>Прибыль</th><th>Дней</th><th>Средняя</th></tr>";

  for (const QVariant &car : ReportOperations::getCarsReport(from, to))
  {
    QVariantList cars = car.toList();
    html += "<tr><td>" + cars[1].toString() + "</td><td>" +
            cars[2].toString() + "</td><td>" +
            cars[3].toString() + "</td><td>" +
            cars[4].toString() + "</td><td>" +
            cars[5].toString() + "</td><td>" +
            cars[6].toString() + "</td><td>" +
            cars[7].toString() + "</td></tr>";
  }

  for (const QVariant &car : ReportOperations::getAllCarsReport(from, to))
  {
    QVariantList cars = car.toList();
    html += "<tr><th>Итого</th><th colspan=2>" +
            cars[0].toString() + "</th><th colspan=2>" +
            cars[1].toString() + "</th><th colspan=2>" +
            cars[2].toString() + "</th></tr>";
  }

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createDriversReport(QDate from, QDate to)
{
  QString title = "Отчет по водителям";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Водитель</th><th>Доход</th><th>Расход</th><th>Прибыль</th></tr>";

  for (const QVariant &driver : ReportOperations::getDriversReport(from, to))
  {
    QVariantList drivers = driver.toList();
    html += "<tr><td>" +
            drivers[1].toString() + "</td><td>" +
            drivers[2].toString() + "</td><td>" +
            drivers[3].toString() + "</td><td>" +
            drivers[4].toString() + "</td></tr>";
  }

  for (const QVariant &driver : ReportOperations::getAllDriversReport(from, to))
  {
    QVariantList drivers = driver.toList();
    html += "<tr><th>Итого</th><th>" +
            drivers[0].toString() + "</th><th>" +
            drivers[1].toString() + "</th><th>" +
            drivers[2].toString() + "</th></tr>";
  }

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createInvestorsReport(QDate from, QDate to)
{
  QString title = "Отчет по инвесторам";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Инвестор</th><th>Доход</th><th>Расход</th><th>Прибыль</th></tr>";

  for (const QVariant &investor : ReportOperations::getInvestorsReport(from, to))
  {
    QVariantList investors = investor.toList();
    html += "<tr><td>" +
            investors[1].toString() + "</td><td>" +
            investors[2].toString() + "</td><td>" +
            investors[3].toString() + "</td><td>" +
            investors[4].toString() + "</td></tr>";
  }

  for (const QVariant &investor : ReportOperations::getAllInvestorsReport(from, to))
  {
    QVariantList investors = investor.toList();
    html += "<tr><th>Итого</th><th>" +
            investors[0].toString() + "</th><th>" +
            investors[1].toString() + "</th><th>" +
            investors[2].toString() + "</th></tr>";
  }

  html += "</table>";

  html += "<p>ECO TAXI</p>";

  createPDF(html, title);
}

void PDFmanager::createTypesReport(QDate from, QDate to)
{
  QString title = "Общий отчет";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Тип</th><th>Доход</th><th>Расход</th><th>Прибыль</th></tr>";

  for (const QVariant &type : ReportOperations::getTypesReport(from, to))
  {
    QVariantList types = type.toList();
    html += "<tr><td>" +
            types[1].toString() + "</td><td>" +
            types[2].toString() + "</td><td>" +
            types[3].toString() + "</td><td>" +
            types[4].toString() + "</td></tr>";
  }

  for (const QVariant &type : ReportOperations::getAllTypesReport(from, to))
  {
    QVariantList types = type.toList();
    html += "<tr><th>Итого</th><th>" +
            types[0].toString() + "</th><th>" +
            types[1].toString() + "</th><th>" +
            types[2].toString() + "</th></tr>";
  }

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createLocationsReport(QDate from, QDate to)
{
  QString title = "Отчет по локациям";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Название</th><th>KWH</th></tr>";

  for (const QVariant &location : ReportOperations::getLocationsReport(from, to))
  {
    QVariantList locations = location.toList();
    html += "<tr><td>" +
            locations[1].toString() + "</td><td>" +
            locations[2].toString() + "</td></tr>";
  }

  for (const QVariant &location : ReportOperations::getAllLocationsReport(from, to))
  {
    QVariantList locations = location.toList();
    html += "<tr><th>Итого</th><th>" +
            locations[0].toString() + "</th></tr>";
  }

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createChargesReport(QDate from, QDate to)
{
  QString title = "Отчет по зарядкам";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>ID Машины</th><th>KWH</th><th>Время</th></tr>";

  for (const QVariant &charge : ReportOperations::getChargesReport(from, to))
  {
    QVariantList charges = charge.toList();
    html += "<tr><td>" + charges[0].toString() + "</td><td>" +
            charges[1].toString() + "</td><td>" +
            charges[2].toString() + "</td></tr>";
  }

  for (const QVariant &charge : ReportOperations::getAllChargesReport(from, to))
  {
    QVariantList charges = charge.toList();
    html += "<tr><th>Итого</th><th></th><th>" +
            charges[0].toString() + "</th></tr>";
  }

  html += "</table>";

  createPDF(html, title);
}


void PDFmanager::createUsersReport(QDate from, QDate to) {
  QString title = "Отчет по пользователям";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>ID</th><th>Имя</th><th>Описание</th></tr>";

  for (const QVariant &user : ReportOperations::getUsersReport(from, to)) {
    QVariantList users = user.toList();
    html += "<tr><td>" + users[1].toString() + "</td><td>" +
            users[2].toString() + "</td><td>" +
            users[3].toString() + "</td></tr>";
  }

  for (const QVariant &user : ReportOperations::getAllUsersReport(from, to)) {
    QVariantList users = user.toList();
    html += "<tr><th>Итого</th><th></th><th>" +
            users[0].toString() + "</th></tr>";
  }

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createCarReport(QDate from, QDate to, int id)
{
  QString title = "Отчет по машине " + QString::number(id);
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Дата</th><th>Тип</th><th>Водитель</th><th>Сумма</th><th>Описание</th></tr>";

  for (const QVariant &car : ReportOperations::getCarReport(id, from, to))
  {
    QVariantList cars = car.toList();
    html += "<tr><td>" + cars[0].toDateTime().toString("dd.MM.yyyy HH:mm:ss") + "</td><td>" +
            cars[1].toString() + "</td><td>" +
            cars[2].toString() + "</td><td>" +
            cars[3].toString() + "</td><td>" +
            cars[4].toString() + "</td></tr>";
  }

  QVariantList car = ReportOperations::getAllCarReport(id, from, to);
  qDebug() << car;
  html += "<tr><th>Итого</th><th></th><th>" +
          car[0].toString() + "</th><th>" +
          car[1].toString() + "</th><th>" +
          car[2].toString() + "</th></tr>";

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createDriverReport(QDate from, QDate to, int id)
{
  QString title = "Отчет по водителю " + Operations::getDriver(id).getName() + " (" + QString::number(id) + ")";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Дата</th><th>Тип</th><th>Машина</th><th>Сумма</th><th>Описание</th></tr>";

  for (const QVariant &driver : ReportOperations::getDriverReport(id, from, to))
  {
    QVariantList drivers = driver.toList();
    html += "<tr><td>" + drivers[0].toDateTime().toString("dd.MM.yyyy HH:mm:ss") + "</td><td>" +
            drivers[1].toString() + "</td><td>" +
            drivers[2].toString() + "</td><td>" +
            drivers[3].toString() + "</td><td>" +
            drivers[4].toString() + "</td></tr>";
  }

  QVariantList driver = ReportOperations::getAllDriverReport(id, from, to);
  html += "<tr><th>Итого</th><th></th><th>" +
          driver[0].toString() + "</th><th>" +
          driver[1].toString() + "</th><th>" +
          driver[2].toString() + "</th></tr>";

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createInvestorReport(QDate from, QDate to, int id)
{
  QString title = "Отчет по инвестору " + Operations::getInvestor(id).getName() + " (" + QString::number(id) + ")";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Машина</th><th>Доход</th><th>Расход</th><th>Прибыль</th></tr>";

  for (const QVariant &investor : ReportOperations::getInvestorReport(id, from, to))
  {
    QVariantList investors = investor.toList();
    html += "<tr><td>" + investors[1].toString() + "</td><td>" +
            investors[2].toString() + "</td><td>" +
            investors[3].toString() + "</td><td>" +
            investors[4].toString() + "</td></tr>";
  }

  QVariantList investor = ReportOperations::getAllInvestorReport(id, from, to);
  html += "<tr><th>Итого</th><th>" +
          investor[1].toString() + "</th><th>" +
          investor[2].toString() + "</th><th>" +
          investor[3].toString() + "</th></tr>";

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createTypeReport(QDate from, QDate to, int id)
{
  QString title = "Отчет по типу " + Operations::getType(id).getName() + " (" + QString::number(id) + ")";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Дата</th><th>Водитель</th><th>Машина</th><th>Сумма</th><th>Описание</th></tr>";

  for (const QVariant &type : ReportOperations::getTypeReport(id, from, to))
  {
    QVariantList types = type.toList();
    html += "<tr><td>" + types[0].toDateTime().toString("dd.MM.yyyy HH:mm:ss") + "</td><td>" +
            types[1].toString() + "</td><td>" +
            types[2].toString() + "</td><td>" +
            types[3].toString() + "</td><td>" +
            types[4].toString() + "</td></tr>";
  }

  QVariantList type = ReportOperations::getAllTypeReport(id, from, to);
  html += "<tr><th>Итого</th><th></th><th>" +
          type[0].toString() + "</th><th>" +
          type[1].toString() + "</th><th>" +
          type[2].toString() + "</th></tr>";

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createLocationReport(QDate from, QDate to, int id)
{
  QString title = "Отчет по локации " + Operations::getLocation(id).getName() + " (" + QString::number(id) + ")";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Дата</th><th>Машина</th><th>Водитель</th><th>KWH</th><th>Время</th>";

  for (const QVariant &location : ReportOperations::getLocationReport(id, from, to))
  {
    QVariantList locations = location.toList();
    html += "<tr><td>" + locations[0].toDateTime().toString("dd.MM.yyyy HH:mm:ss") + "</td><td>" +
            locations[1].toString() + "</td><td>" +
            locations[2].toString() + "</td><td>" +
            locations[3].toString() + "</td><td>" +
            locations[4].toString() + "</td></tr>";
  }

  QVariantList location = ReportOperations::getAllLocationReport(id, from, to);
  html += "<tr><th colspan=2>Итого</th><th colspan=3>" +
          location[0].toString() + "</th></tr>";

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createChargeReport(QDate from, QDate to, int id)
{
  QString title = "Отчет по зарядкам для машины " + QString::number(id);
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>Дата</th><th>Водитель</th><th>Локация</th><th>KWH</th><th>Время</th>";

  for (const QVariant &charge : ReportOperations::getChargesByCarReport(id, from, to))
  {
    QVariantList charges = charge.toList();
    html += "<tr><td>" + charges[0].toDateTime().toString("dd.MM.yyyy HH:mm:ss") + "</td><td>" +
            charges[1].toString() + "</td><td>" +
            charges[2].toString() + "</td><td>" +
            charges[3].toString() + "</td><td>" +
            charges[4].toString() + "</td></tr>";
  }

  QVariantList charge = ReportOperations::getAllChargesByCarReport(id, from, to);
  html += "<tr><th colspan=2>Итого</th><th colspan=3>" +
          charge[0].toString() + "</th></tr>";

  html += "</table>";

  createPDF(html, title);
}

void PDFmanager::createUserReport(QDate from, QDate to, int id)
{
    QString title = "Отчет по пользователю  " + Operations::getUser(id).getName() + "(" + QString::number(id) + ")";
  QString html = "<h2>" + title + "</h2>";
  html += "<p>" + from.toString("dd.MM.yyyy") + " - " + to.toString("dd.MM.yyyy") + "</p><br>";
  html += "<table width=100%><tr><th>ID</th><th>Дата</th></tr>";

  for (const QVariant &user : ReportOperations::getUserReport(id, from, to))
  {
    QVariantList users = user.toList();
    html += "<tr><td>" + users[0].toString() + "</td><td>" +
            users[1].toDateTime().toString("dd.MM.yyyy HH:mm:ss") + "</td></tr>";
  }

  QVariantList user = ReportOperations::getAllUserReport(id, from, to);
  html += "<tr><th>Итого</th><th>" +
          user[0].toList()[0].toString() + "</th></tr>";

  html += "</table>";

  createPDF(html, title);
}
