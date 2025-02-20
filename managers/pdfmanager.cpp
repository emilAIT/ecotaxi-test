#include "pdfmanager.h"
#include "reportoperations.h"

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

QList<QDate> PDFmanager::generateDateRange(const QString &dateRange)
{
    QList<QDate> dates;

    // Разделяем строку на начальную и конечную даты
    QStringList dateParts = dateRange.split(" - ");
    if (dateParts.size() != 2) {
        qWarning() << "Неверный формат даты";
        return dates;
    }

    // Преобразуем строки в объекты QDate
    QDate startDate = QDate::fromString(dateParts[0], "dd.MM.yyyy");
    QDate endDate = QDate::fromString(dateParts[1], "dd.MM.yyyy");

    // Проверяем корректность дат
    if (!startDate.isValid() || !endDate.isValid()) {
        qWarning() << "Неверные даты";
        return dates;
    }

    // Генерируем список всех дат между startDate и endDate
    QDate currentDate = startDate;
    while (currentDate <= endDate) {
        dates.append(currentDate);
        currentDate = currentDate.addDays(1); // Переходим к следующему дню
    }

    return dates;
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
void PDFmanager::ToPDF2(QString title, QString dates, int start)
{
    QString html = "<h2>" + title + "</h2>";
    html += "<p>" + dates + "</p><br>";

    QList<QDate> date = generateDateRange(dates);

    for(int i = 0; i < date.size(); i++) {
        QStandardItemModel *model = new QStandardItemModel();
        QDate d = date[i];

        model->setHorizontalHeaderLabels({"carId", "ID", "Инвестор", "Доход", "Налог 10%", "KWH x 10", "Расход", "Общий", "Дней", ">0", "Средняя", "%", "Комиссия", "Инвестору"});
        for (const QVariant &car : ReportOperations::getCarsReport(d,d))
        {
            QVariantList cars = car.toList();
            QList<QStandardItem *> row;

            // Assume that the data in cars[] is in the correct order and types.
            row.append(new QStandardItem(cars[0].toString()));  // carId
            row.append(new QStandardItem(cars[1].toString()));  // carSid
            row.append(new QStandardItem(cars[2].toString()));  // investorName

            // Ensure numerical data is set correctly for sorting
            QStandardItem *incomeItem = new QStandardItem();
            incomeItem->setData(cars[3].toInt(), Qt::DisplayRole);
            row.append(incomeItem); // income

            QStandardItem *taxItem = new QStandardItem();
            taxItem->setData(cars[4].toInt(), Qt::DisplayRole);
            row.append(taxItem); // tax

            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(cars[5].toInt(), Qt::DisplayRole);
            row.append(kwhItem); // kwh * 10

            QStandardItem *outcomeItem = new QStandardItem();
            outcomeItem->setData(cars[6].toInt(), Qt::DisplayRole);
            row.append(outcomeItem); // outcome

            QStandardItem *profitItem = new QStandardItem();
            profitItem->setData(cars[7].toInt(), Qt::DisplayRole);
            row.append(profitItem); // profit

            QStandardItem *daysItem = new QStandardItem();
            daysItem->setData(cars[8].toInt(), Qt::DisplayRole);
            row.append(daysItem); // daysWorked

            QStandardItem *daysItem2 = new QStandardItem();
            daysItem2->setData(cars[9].toInt(), Qt::DisplayRole);
            row.append(daysItem2); // nonZeroDays

            QStandardItem *averageItem = new QStandardItem();
            averageItem->setData(cars[10].toInt(), Qt::DisplayRole);
            row.append(averageItem); // averageProfitPerDay

            QStandardItem *percentageItem = new QStandardItem();
            percentageItem->setData(cars[11].toInt(), Qt::DisplayRole);
            row.append(percentageItem); // percentage

            QStandardItem *ourIncomeItem = new QStandardItem();
            ourIncomeItem->setData(cars[12].toInt(), Qt::DisplayRole);
            row.append(ourIncomeItem); // ourIncome

            QStandardItem *investorsIncomeItem = new QStandardItem();
            investorsIncomeItem->setData(cars[13].toInt(), Qt::DisplayRole);
            row.append(investorsIncomeItem); // investorsIncome

            model->appendRow(row);
        }


        html += modelToHTML2(model, d);
        if(i < date.size() - 1) {
            html += "<div style='page-break-after: always;'></div>";
        }




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

QString PDFmanager::modelToHTML2(QStandardItemModel *model, QDate date,int start)
{
    QString html;

    // Remove margin, and set table width to 100%

    html += "<table style='margin: 0;' margin=0 width=100%><caption style='color:green; font-size:30px; text-align:center;'>" + date.toString("dd.MM.yyyy") + "</caption><tr>";


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
            // Get the item at the position (i, j)
            QStandardItem *item = model->item(i, j);
            QString cellData = item ? item->text() : "";  // Get the text of the item, or an empty string if null
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
