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
  -webkit-tap-highlight-color: transparent;
}
*:focus {
  outline: none;
}
html,body,div,span,applet,object,iframe,h1,h2,h3,h4,h5,h6,p,blockquote,pre,a,abbr,acronym,address,big,cite,code,del,dfn,em,img,ins,kbd,q,s,samp,span,
small,strike,strong,sub,sup,tt,var,b,u,i,dl,dt,dd,ol,ul,li,fieldset,form,label,legend,table,caption,tbody,
tfoot,thead,tr,th,td,article,aside,canvas,details,embed,figure,figcaption,footer,header,hgroup,menu,nav,output,ruby,section,summary,time,mark,audio,
video {
  margin: 0;
  padding: 0;
  border: 0;
  vertical-align: baseline;
}
article,aside,details,figcaption,figure,footer,header,hgroup,menu,nav,
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

// void PDFmanager::ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
// {
//     QString html = "<h2>" + title + "</h2>";
//     html += "<p>" + dates + "</p><br>";

//     QMap<QString, QString> groupedHTML;  // Stores data grouped by date

//     for (int i = 0; i < models.size(); i++)
//     {
//         QAbstractItemModel *model = models[i];

//         // Extract date from model (assuming date is in the first column)
//         QString currentDate = model->index(0, 0).data(Qt::DisplayRole).toString();

//         // Append data for this date
//         if (!groupedHTML.contains(currentDate))
//         {
//             groupedHTML[currentDate] = "";  // Initialize entry for this date
//         }

//         groupedHTML[currentDate] += modelToHTML(model, start != 0 && i == 0 ? 1 : 0);
//     }

//     // Construct final HTML with page breaks
//     bool firstPage = true;
//     for (auto it = groupedHTML.begin(); it != groupedHTML.end(); ++it)
//     {
//         if (!firstPage)
//         {
//             html += "<div style='page-break-before: always'></div>";  // Page break between different days
//         }
//         firstPage = false;

//         html += "<h3>Date: " + it.key() + "</h3>";  // Add section title with date
//         html += it.value();  // Append the corresponding table for this date
//     }

//     createPDF(html, title + " " + dates);
// }


// void PDFmanager::ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
// {
//     QString html = "<h2>" + title + "</h2>";
//     html += "<p>" + dates + "</p><br>";

//     QMap<QString, QString> groupedHTML;  // Stores data grouped by date

//     for (int i = 0; i < models.size(); i++)
//     {
//         QAbstractItemModel *model = models[i];

//         // Ensure the first column contains valid date data
//         QVariant dateVariant = model->index(0, 0).data(Qt::DisplayRole);
//         if (!dateVariant.isValid()) continue; // Skip invalid rows

//         QString currentDate = dateVariant.toString().trimmed();

//         // Append data for this date
//         if (!groupedHTML.contains(currentDate))
//         {
//             groupedHTML[currentDate] = "";  // Initialize entry for this date
//         }

//         groupedHTML[currentDate] += modelToHTML(model, start != 0 && i == 0 ? 1 : 0);
//     }

//     // Construct final HTML with page breaks
//     bool firstPage = true;
//     for (auto it = groupedHTML.begin(); it != groupedHTML.end(); ++it)
//     {
//         if (!firstPage)
//         {
//             html += "<div style='page-break-before: always'></div>";  // Ensure new page for each day
//         }
//         firstPage = false;

//         html += "<h3>Date: " + it.key() + "</h3>";  // Add section title with date
//         html += it.value();  // Append the corresponding table for this date
//     }

//     createPDF(html, title + " " + dates);
// }



// void PDFmanager::ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
// {
//     QString html;

//     // Extract start and end dates from user selection
//     QStringList dateRange = dates.split(" - ");
//     if (dateRange.size() != 2) return; // Invalid range check

//     QDate startDate = QDate::fromString(dateRange[0], "dd.MM.yyyy");
//     QDate endDate = QDate::fromString(dateRange[1], "dd.MM.yyyy");

//     if (!startDate.isValid() || !endDate.isValid() || startDate > endDate) return;

//     // Prepare a map to store data grouped by date
//     QMap<QString, QString> groupedHTML;

//     // Loop through models and extract data for each date
//     for (int i = 0; i < models.size(); i++)
//     {
//         QAbstractItemModel *model = models[i];

//         for (int row = 0; row < model->rowCount(); row++) // Loop through all rows
//         {
//             QVariant dateVariant = model->index(row, 0).data(Qt::DisplayRole); // Assuming date is in column 0
//             if (!dateVariant.isValid()) continue;

//             QString currentDate = dateVariant.toString().trimmed();

//             if (!groupedHTML.contains(currentDate))
//             {
//                 groupedHTML[currentDate] = "";  // Initialize entry for this date
//             }

//             groupedHTML[currentDate] += modelToHTML(model, start != 0 && i == 0 ? 1 : 0);
//         }
//     }

//     // Generate PDF with all dates, ensuring each has a separate page
//     bool firstPage = true;
//     QDate currentDate = startDate;

//     while (currentDate <= endDate)
//     {
//         if (!firstPage)
//         {
//             html += "<div style='page-break-before: always'></div>";  // Page break before each date
//         }
//         firstPage = false;

//         QString dateString = currentDate.toString("dd.MM.yyyy");

//         html += "<h3>Дата: " + dateString + "</h3>";

//         // If there is data for this date, add it
//         if (groupedHTML.contains(dateString))
//         {
//             html += groupedHTML[dateString];  // Add table data
//         }
//         else
//         {
//             html += "<p style='text-align: center; color: #777;'>Нет данных за эту дату</p>";
//         }

//         // Move to the next day
//         currentDate = currentDate.addDays(1);
//     }

//     // Generate the PDF
//     createPDF(html, title + " " + dates);
// }



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

void PDFmanager::exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start)
{
    ColumnSelectionDialog dialog(models, title, dates, start);
    
    dialog.exec();
}


//

// void PDFmanager::exportDailyReport(QAbstractItemModel *model) {
//     if (!model || model->rowCount() == 0) {
//         QMessageBox::warning(nullptr, "Ошибка", "Модель данных пуста!");
//         return;
//     }


//     QApplication::setOverrideCursor(Qt::WaitCursor);
//     QString title = "Отчет_по_Дням_" + QDate::currentDate().toString("dd.MM.yyyy");

//     // Открываем окно выбора колонок
//     // ColumnSelectionDialog dialog({model}, "Выбор колонок для ежедневного отчета", QDate::currentDate().toString("dd.MM.yyyy"), 0);
//     // if (dialog.exec() != QDialog::Accepted) {
//     //     QApplication::restoreOverrideCursor();
//     //     return;
//     // }

//     // Получаем отфильтрованную модель
//     QAbstractItemModel* filteredModel = model;

//     // Определяем, в каком столбце находится дата
//     int dateColumnIndex = -1;
//     for (int j = 0; j < filteredModel->columnCount(); j++) {
//         QString header = filteredModel->headerData(j, Qt::Horizontal).toString().toLower();
//         if (header.contains("дата") || header.contains("date")) {
//             dateColumnIndex = j;
//             break;
//         }
//     }
//     if (dateColumnIndex == -1) {
//         QMessageBox::warning(nullptr, "Ошибка", "Не найдена колонка с датой!");
//         QApplication::restoreOverrideCursor();
//         return;
//     }

//     // Группируем и сортируем данные по дате
//     QMap<QDate, QList<QList<QVariant>>> groupedData;
//     for (int i = 0; i < filteredModel->rowCount(); i++) {
//         QVariant dateVariant = filteredModel->index(i, dateColumnIndex).data(Qt::DisplayRole);
//         QDate date = QDate::fromString(dateVariant.toString(), "yyyy-MM-dd");
//         if (!date.isValid()) date = dateVariant.toDate();
//         if (!date.isValid()) continue;

//         QList<QVariant> row;
//         for (int j = 0; j < filteredModel->columnCount(); j++) {
//             row.append(filteredModel->index(i, j).data(Qt::DisplayRole));
//         }
//         groupedData[date].append(row);
//     }

//     QList<QDate> sortedDates = groupedData.keys();
//     std::sort(sortedDates.begin(), sortedDates.end());

//     // Генерация HTML с разбиением по дням на разные страницы
//     QString html;
//     bool firstPage = true;
//     for (const QDate& date : sortedDates) {
//         if (!firstPage) {
//             html += "<div style='page-break-before: always'></div>";  // Разрыв страницы после первой
//         }
//         firstPage = false;

//         html += "<h2>Отчет за " + date.toString("dd.MM.yyyy") + "</h2>";
//         html += generateTableHtml(groupedData[date], filteredModel);
//     }

//     createPDF(html, title);
//     QApplication::restoreOverrideCursor();
// }





// QString PDFmanager::generateTableHtml(const QList<QList<QVariant>>& data, QAbstractItemModel* model) {
//     QString html;
//     html += "<table style='width: 100%; border-collapse: collapse; margin: 0 auto;'>";
//     html += "<thead><tr>";

//     // Добавляем заголовки
//     for (int j = 0; j < model->columnCount(); j++) {
//         QString header = model->headerData(j, Qt::Horizontal).toString();
//         html += "<th style='border: 1px solid black; padding: 5px; background-color: #f2f2f2;'>" + header + "</th>";
//     }
//     html += "</tr></thead><tbody>";

//     // Добавляем строки
//     for (const QList<QVariant>& row : data) {
//         html += "<tr>";
//         for (const QVariant& cell : row) {
//             html += "<td style='border: 1px solid black; padding: 5px; text-align: center;'>" + cell.toString() + "</td>";
//         }
//         html += "</tr>";
//     }
//     html += "</tbody></table>";
//     return html;
// }


