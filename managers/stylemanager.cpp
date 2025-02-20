#include "stylemanager.h"

styleManager::styleManager(QApplication *app)
{
    QVariantMap vars;
    vars["mainColor"] = "#007700";
    vars["mainHoverColor"] = "#005500";
    vars["backgroundColor"] = "#eeeeee";
    vars["backgroundHoverColor"] = "#aaaaaa";
    vars["textColor"] = "#222222";
    vars["grayColor"] = "#b9b9b9";

    vars["btnFontSize"] = "28px";
    vars["textFontSize"] = "18px";
    vars["editFontSize"] = "18px";

    vars["bgBorderRadius"] = "10px";
    vars["btnBorderRadius"] = "6px";
    vars["editBorderRadius"] = "8px";

    QString styles = R"A(
* {
    font-family: Inter Black;
}

/* #################################################### MAIN STYLES #################################################### */

QWidget {
    background-color: #FFFDD0;
}

QLabel {
    background: transparent;
    color: )A" +
                     vars["textColor"].toString() + R"A(;
    font-size: )A" +
                     vars["textFontSize"].toString() + R"A(;
}

QLabel[color="white"] {
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

.QFrame {
    background-color: transparent;
}

.QFrame#greenBack, QFrame[color="green"] {
    background: qlineargradient(
        x1: 1, y1: 0, x2: 0, y2: 1,
        stop: 0 #000000,
        stop: 0.5 #008f11,
        stop: 1 #006400
    );
    border-radius: )A" +
                     vars["bgBorderRadius"].toString() + R"A(;
    border: 2px solid black;

}

/* #################################################### BUTTONS STYLES #################################################### */

QPushButton {
    background: qlineargradient(
        x1: 0, y1: 0, x2: 1, y2: 1,  /* Diagonal Direction */
        stop: 0 rgba(0, 0, 0, 0.7),    /* Black (70% Opacity) */
        stop: 0.3 rgba(0, 100, 0, 0.75),  /* Dark Green (75% Opacity) */
        stop: 0.7 rgba(0, 143, 17, 0.95), /* Dark Neon Green (95% Opacity) */
        stop: 1 rgba(0, 0, 0, 0.7)     /* Black (70% Opacity) - Corners */
    );
    color: white;
    font: 900 )A" +
                     vars["btnFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["btnBorderRadius"].toString() + R"A(;
    padding: 6px;
    border: 2px solid black;
}

QPushButton:hover {
    background-color: )A" +
                     vars["mainHoverColor"].toString() + R"A(;    background: qlineargradient(
        x1: 0, y1: 0, x2: 1, y2: 1,  /* Diagonal Direction */
        stop: 0 rgba(0, 0, 0, 0.5),    /* Black (70% Opacity) */
        stop: 0.3 rgba(0, 100, 0, 0.95),  /* Dark Green (75% Opacity) */
        stop: 0.7 rgba(0, 143, 17, 0.6), /* Dark Neon Green (95% Opacity) */
        stop: 1 rgba(0, 0, 0, 0.6)     /* Black (70% Opacity) - Corners */
    );
    border: 2px solid lime;
}

QPushButton:pressed {
    background: qlineargradient(
        x1: 0, y1: 0, x2: 1, y2: 1,  /* Diagonal Direction */
        stop: 0 rgba(0, 0, 0, 1),    /* Black (70% Opacity) */
        stop: 0.3 rgba(0, 100, 0, 0.95),  /* Dark Green (75% Opacity) */
        stop: 0.7 rgba(0, 143, 17, 1), /* Dark Neon Green (95% Opacity) */
        stop: 1 rgba(0, 0, 0, 0.6)     /* Black (70% Opacity) - Corners */
    );
    border: 2px solid black;
}

QPushButton:flat {
    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
        stop:0 rgba(0,0,0,0.4),
        stop:0.4 black,
        stop:0.5 rgba(255,255,255,0.6),
        stop:0.6 black,
        stop:1 rgba(0,0,0,0.4));

    color: white;
}

QPushButton:flat:hover {
    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
        stop:0 black,
        stop:0.5 white,
        stop:1 black);

    color: black;
}

QPushButton:flat:disabled{
    background-color: transparent;
    border: none;
    color: transparent;
}

QPushButton:flat:pressed {
    background-color: )A" +
                     vars["textColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QPushButton:disabled {
    background-color: transparent;
    color: transparent;
    border: none;
}

QPushButton[color="gray"] {
    background: qradialgradient(
        cx: 0.5, cy: 0.5, radius: 1,
        stop: 0 rgb(0, 0, 0),
        stop: 0.5 rgba(128, 128, 128, 0.9),
        stop: 1 rgb(80, 80, 80)
    );
    color: white;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 4px;
}

QPushButton[color="gray"]:hover {
    background: qradialgradient(
        cx: 0.5, cy: 0.5, radius: 1,
        stop: 0 rgba(0, 0, 0, 0.4),
        stop: 0.5 rgba(128, 128, 128, 0.4),
        stop: 1 rgba(80, 80, 80, 0.7)
    );
}

QPushButton[color="gray"]:disabled {
    background: none;
    color: transparent;
    border: none;
}

/* #################################################### EDITS STYLES #################################################### */

QLineEdit {
    background-color: rgba(0,0,0,0.7);
    color: white;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    font-size: )A" +
                     vars["editFontSize"].toString() + R"A(;
    border: 2px solid gray;
}

QLineEdit:hover {
    border: 2px solid lime;
}

QLineEdit[color="red"] {
    background-color: red;
}

QTextEdit {
    background-color: rgba(0,0,0,0.7);
    color: white;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    font-size: )A" +
                     vars["editFontSize"].toString() + R"A(;
    border: 2px solid gray;
}

QTextEdit:hover {
    border: 2px solid lime;
}

/* #################################################### LABELS STYLES #################################################### */

QLabel#Header {
    font: 900 )A" +
                     vars["btnFontSize"].toString() + R"A(;
}

QLabel#EcoTaxi {
    font: 900 80px;
}

QLabel#EcoTaxiWhite {
    font: 900 50px;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QLabel {
    font-weight: 900;
}

QLabel[color="white"] {
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QLabel#warningLabel {
    color: red;
}

/* #################################################### COMBOBOX STYLES #################################################### */

QComboBox {
    background-color: rgba(0,0,0,0.7);
    color: white;
    font: bold )A" +
                     vars["editFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    border: 2px solid gray;
}

QComboBox:hover {
    border: 2px solid lime;
}


QComboBox::drop-down:button {
    background-color: rgba(0,0,0,0);
    width: )A" +
                     vars["editFontSize"].toString() + R"A(;
}


QComboBox::down-arrow {
    width: 0;
    height: 0;
    border-left: 7px solid black;
    border-right: 7px solid black;
    right: 10px;
    border-top: 10px solid white;
}


QComboBox::item {
    color: white;
    padding: 5px;
    border: 1px solid black;
}


QComboBox::down-arrow:on {
    border-top: none;
    border-bottom: 10px solid white;
}

QListView {
    padding: 5px;
    outline: 0;
    color: white;
    background-color: rgba(0,0,0,0.7);
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    border: none;
}

QListView::item {
    color: white;
    border-radius: 5px;
    padding: 5px;
    border: none;
}

QListView::item:selected {
    color: white;
    border: none;
}

/* #################################################### NAVIGATION COMBOBOX STYLES #################################################### */

QComboBox[type="nav"] {
    background: qlineargradient(
        x1: 0, y1: 0, x2: 1, y2: 1,
        stop: 0 rgba(0, 0, 0, 0.7),
        stop: 0.3 rgba(0, 100, 0, 0.75),
        stop: 0.7 rgba(0, 143, 17, 0.95),
        stop: 1 rgba(0, 0, 0, 0.7)
    );
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    font: bold )A" +
                     vars["btnFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["btnBorderRadius"].toString() + R"A(;
    padding: 0 20px;
    border: 2px solid black;
}

QComboBox[type="nav"]:hover {
    background: qlineargradient(
        x1: 0, y1: 0, x2: 1, y2: 1,
        stop: 0 rgba(0, 0, 0, 0.5),
        stop: 0.3 rgba(0, 100, 0, 0.95),
        stop: 0.7 rgba(0, 143, 17, 0.6),
        stop: 1 rgba(0, 0, 0, 0.6)
    );
    border: 2px solid lime;
}

QComboBox[type="nav"]::drop-down:button {
    background-color: #00000000;
}

QComboBox[type="nav"]::down-arrow {
    border: none;
}

QComboBox[type="nav"]::down-arrow:on {
    border: none;
}

QComboBox[type="nav"] QListView {
    outline: 0;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    background: qlineargradient(
        x1: 0, y1: 0, x2: 1, y2: 1,  /* Diagonal Direction */
        stop: 0 rgba(0, 0, 0, 0.7),    /* Black (70% Opacity) */
        stop: 0.3 rgba(0, 100, 0, 0.75),  /* Dark Green (75% Opacity) */
        stop: 0.7 rgba(0, 143, 17, 0.95), /* Dark Neon Green (95% Opacity) */
        stop: 1 rgba(0, 0, 0, 0.7)     /* Black (70% Opacity) - Corners */
    );
    border-radius: )A" +
                     vars["btnBorderRadius"].toString() + R"A(;
    border: none;
}

QComboBox[type="nav"] QListView::item {
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    border: none;
}

QComboBox[type="nav"] QListView::item:selected {
    color: )A" +
                     vars["mainColor"].toString() + R"A(;
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    border: none;
}

QComboBox[type="nav"] QListView::item:selected:hover{
    border: 2px solid black;
}

/* #################################################### TABLE STYLES #################################################### */

QTableView {
    font-family: Inter;
    font-size: 18px;
    gridline-color: #b9b9b9;
    alternate-background-color: #ecf0f1;
    background-color: white;
    font-weight: normal;
    color: black;
    border: 2px solid black;
}

QHeaderView::section {
    background-color: black;
    color: white;
    font-weight: bold;
    padding: 5px;
    border: 2px solid gray;
    font-family: Inter;
    font-size: 20px;
    text-align: center;
}

QTableView::item:selected {
    background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

/* #################################################### SCROLLBAR STYLES #################################################### */

QScrollBar:horizontal {
    height: 15px;
    background: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QScrollBar::handle:horizontal {
    min-width: 20px;
    background: )A" +
                     vars["mainColor"].toString() + R"A(;
}

QScrollBar:vertical {
    width: 15px;
    background: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QScrollBar::handle:vertical {
    min-height: 20px;
    background: )A" +
                     vars["mainColor"].toString() + R"A(;
}

/* #################################################### CALENDAR STYLES #################################################### */

QToolButton {
    background-color: )A" +
            vars["mainColor"].toString() + R"A(;
}

QCalendarWidget QToolButton {
    height: 60px;
    width: 180px;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    font-size: 24px;
    icon-size: 64px;
    background-color: white;
    color: )A" + vars["mainColor"].toString() + R"A(;
    border: none;
}
QCalendarWidget QWidget#qt_calendar_navigationbar {
    background-color: white; /* Background color for the navigation bar */
}
QCalendarWidget QMenu {
    width: 150px;
    left: 20px;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    font-size: )A" +
                vars["textFontSize"].toString() + R"A(;
}
QCalendarWidget QSpinBox {
    width: 150px;
    font-size: 24px;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    selection-background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    selection-color: )A" +
                     vars["textColor"].toString() + R"A(;
}

QCalendarWidget QSpinBox::up-button {
    subcontrol-origin: border;
    subcontrol-position: top right;
    width:65px;
}
QCalendarWidget QSpinBox::down-button {
    subcontrol-origin: border;
    subcontrol-position: bottom right;
    width:65px;
}
QCalendarWidget QSpinBox::up-arrow {
    width:56px;
    height:56px;
}
QCalendarWidget QSpinBox::down-arrow {
    width:56px;
    height:56px;
}

/* header row */
QCalendarWidget QWidget {
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    alternate-background-color: )A" +
                                vars["mainColor"].toString() + R"A(;
}

/* normal days */
QCalendarWidget QAbstractItemView:enabled
{
    font-size:24px;
    color: )A" +
            vars["textColor"].toString() + R"A(;
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    selection-background-color: )A" +
                                vars["backgroundColor"].toString() + R"A(;
    selection-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    selection-border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
}

/* days in other months */
/* navigation bar */
QCalendarWidget QWidget#qt_calendar_navigationbar
{
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QCalendarWidget QAbstractItemView:disabled
{
    color: )A" +
            vars["grayColor"].toString() + R"A(;
}

/* #################################################### CONTEXT MENU STYLES #################################################### */

QMenu::item
{
    color: )A" +
            vars["textColor"].toString() + R"A(;
}

QMenu
{
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
}

/* #################################################### TIME EDIT STYLES #################################################### */

QTimeEdit
{
    background-color: #FFFDD0;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    font-size: )A" +
               vars["btnFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    border-width: 4px;
    spacing: 5px;
}

QTimeEdit:disabled
{
    color: )A" +
            vars["backgroundColor"].toString() + R"A(;
}

QTimeEdit::up-button {
    subcontrol-origin: border;
    subcontrol-position: top right;
    width: 20px;
    background-color: #00000000;
}
QTimeEdit::down-button {
    subcontrol-origin: border;
    subcontrol-position: bottom right;
    width: 20px;
    background-color: #00000000;
}
QTimeEdit::up-arrow {
    width: 0;
    height: 0;
    border-left: 10px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-right: 10px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-bottom: 15px solid )A" +
                     vars["mainColor"].toString() + R"A(;
}
QTimeEdit::down-arrow {
    width: 0;
    height: 0;
    border-left: 10px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-right: 10px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-top: 15px solid )A" +
                     vars["mainColor"].toString() + R"A(;
}

QTimeEdit::up-arrow:disabled
{
    border: none;
}
QTimeEdit::down-arrow:disabled
{
    border: none;
}

QDateTimeEdit {
    font-size: 16px;
    background-color: #FFFDD0;
    color: )A" + vars["textColor"].toString() + R"A(;
    border-radius: )A" + vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    font-size: )A" + vars["editFontSize"].toString() + R"A(;
}

QDateTimeEdit::drop-down {
    background-color: white;
    border: 1px solid black;
}

QDateTimeEdit::down-arrow {
    width: 0;
    height: 0;
}

QDateTimeEdit::up-arrow {
    width: 0;
    height: 0;
}

QDateTimeEdit::up-button, QDateTimeEdit::down-button {
    subcontrol-origin: border;
    background-color: #00000000;
    width: 20px;
}

QDateTimeEdit:disabled {
    background-color: #FFFDD0;
    color: )A" + vars["grayColor"].toString() + R"A(;
    border: none;
}


/* #################################################### CHECK BOX STYLES #################################################### */

QCheckBox
{
    background-color: transparent;
    font-size: )A" +
                vars["editFontSize"].toString() + R"A(;
    color: )A" +
            vars["textColor"].toString() + R"A(;

}

QCheckBox:checked
{
    color: )A" +
            vars["mainColor"].toString() + R"A(;
}

QCheckBox::indicator
{
    width: 20px;
    height: 20px;
    background-color: #FFFDD0;
    border-style: solid;
    border-width: 2px;
    border-color: )A" +
                    vars["mainColor"].toString() + R"A(;
    border-radius: 7px;
    border: 1px solid lime;
}

QCheckBox::indicator:checked
{
    background-color: )A" +
                    vars["mainColor"].toString() + R"A(;
    border: 1px solid black;
}

QCheckBox[color="white"]
{
    color: #FFFDD0;
}

QCheckBox:checked[color="white"]
{
    color: #FFFDD0;
}

QCheckBox::indicator[color="white"]
{
    width: 20px;
    height: 20px;
    background-color: )A" +
                    vars["mainColor"].toString() + R"A(;
    border-style: solid;
    border-width: 2px;
    border-color: )A" +
                    vars["backgroundColor"].toString() + R"A(;
}

QCheckBox::indicator:checked[color="white"]
{
    background-color: #FFFDD0;
}

)A";

    app->setStyleSheet(styles);
}
