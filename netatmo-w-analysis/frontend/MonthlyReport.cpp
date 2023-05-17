#include "MonthlyReport.h"

MonthlyReport::MonthlyReport() : QWidget()
{
    _date = new QDate(QDate::currentDate().addMonths(-1));
    dbHandler = new DatabaseHandler("netatmo-w-analysis/netatmo_analysis.db");
    deviceLocale = new QLocale();

    yearMonthPicker = new YearMonthPicker(_date->year(), _date->month());
//    yearMonthPicker->setCalendarPopup(true);
//    yearMonthPicker->setDisplayFormat("MMM yyyy");
//    yearMonthPicker->setCurrentSection(QDateTimeEdit::MonthSection);

    this->setGeometry(300, 40, 720, 950);
    layout = new QGridLayout();

    model = new QStandardItemModel();

    view = new QTableView();
    view->setModel(model);

    model->setHorizontalHeaderLabels(QStringList({"T. min.", "T. max.", "T. moy."}));

    add1MonthButton = new QPushButton(">>>");
    substract1MonthButton = new QPushButton("<<<");

    connect(add1MonthButton, SIGNAL(clicked()), this, SLOT(add1Month()));
    connect(substract1MonthButton, SIGNAL(clicked()), this, SLOT(substract1Month()));

    currentMonthClickableLabel = new QPushButton(_date->toString("MMMM yyyy"));
    currentMonthClickableLabel->setFlat(true);
//    currentMonthClickableLabel->setAlignment(Qt::AlignCenter);
    currentMonthClickableLabel->setFont(QFont("Arial", 14));
    connect(currentMonthClickableLabel, SIGNAL(clicked()), yearMonthPicker, SLOT(show()));

//    rbT = new QRadioButton("Température");
//    rbRh = new QRadioButton("Humidité");
//    rbTd = new QRadioButton("Point de rosée");
//    rbHx = new QRadioButton("Humidex");

//    boxInt = new QCheckBox("Intérieur");

//    connect(rbT, SIGNAL(toggled(bool)), this, SLOT(majType()));
//    connect(rbRh, SIGNAL(toggled(bool)), this, SLOT(majType()));
//    connect(rbTd, SIGNAL(toggled(bool)), this, SLOT(majType()));
//    connect(rbHx, SIGNAL(toggled(bool)), this, SLOT(majType()));
//    connect(boxInt, SIGNAL(stateChanged(int)), this, SLOT(majType()));

    buttonsLayout = new QVBoxLayout();
//    buttonsLayout->addWidget(rbT);
//    buttonsLayout->addWidget(rbRh);
//    buttonsLayout->addWidget(rbTd);
//    buttonsLayout->addWidget(rbHx);
//    buttonsLayout->addWidget(boxInt);

    layout->addWidget(add1MonthButton, 0, 2);
    layout->addWidget(substract1MonthButton, 0, 0);
    layout->addWidget(currentMonthClickableLabel, 0, 1);
    layout->addWidget(view, 1, 0, 3, 2);
    layout->addLayout(buttonsLayout, 1, 2);

    setLayout(layout);
    fillBoard();

//    majDate();



}

//void RapportMensuel::majDate() {

//    for (int jour = 1; jour <= _date->daysInMonth(); jour++) {
//        QString s = _date->toString("dd/MM");
//        s[0] = str(jour / 10)[0];
//        s[1] = str(jour % 10)[0];
//        model->setVerticalHeaderItem(jour-1, new QStandardItem(s));
//    }
//    if (_date->daysInMonth() < 31) model->removeRows(_date->daysInMonth(), model->rowCount() - _date->daysInMonth());
//    if (boxInt->isChecked()) remplirTableauInt();
//    else remplirTableau();

//}

void MonthlyReport::fillBoard() {
    for (int day = 1; day <= _date->daysInMonth(); day++) {
        QDate date = QDate(_date->year(), _date->month(), day);
        double tn = dbHandler->getResultFromDatabase(
                    "SELECT minTemperature FROM OutdoorDailyRecords WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
        double tx = dbHandler->getResultFromDatabase(
                    "SELECT maxTemperature FROM OutdoorDailyRecords WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
        double tm = dbHandler->getResultFromDatabase(
                    "SELECT avgTemperature FROM OutdoorDailyRecords WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();

        model->setVerticalHeaderItem(day - 1, new QStandardItem(date.toString("dd/MM")));
        model->setItem(day - 1, 0, new QStandardItem(deviceLocale->toString(tn, 'f', 1) + " °C"));
        model->item(day - 1, 0)->setBackground(QBrush(temperatureColor(tn)));

        model->setVerticalHeaderItem(day - 1, new QStandardItem(date.toString("dd/MM")));
        model->setItem(day - 1, 1, new QStandardItem(deviceLocale->toString(tx, 'f', 1) + " °C"));
        model->item(day - 1, 1)->setBackground(QBrush(temperatureColor(tx)));

        model->setVerticalHeaderItem(day - 1, new QStandardItem(date.toString("dd/MM")));
        model->setItem(day - 1, 2, new QStandardItem(deviceLocale->toString(tm, 'f', 1) + " °C"));
        model->item(day - 1, 2)->setBackground(QBrush(temperatureColor(tm)));

        model->item(day - 1, 0)->setEditable(false);
        model->item(day - 1, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(day - 1, 1)->setEditable(false);
        model->item(day - 1, 1)->setTextAlignment(Qt::AlignCenter);
        model->item(day - 1, 2)->setEditable(false);
        model->item(day - 1, 2)->setTextAlignment(Qt::AlignCenter);
    }
    // Clean the extra rows when switching from a 31-day month to a shorter one
    model->removeRows(_date->daysInMonth(), 31 - _date->daysInMonth());
}

void MonthlyReport::destroy() {
    delete this;
}

QColor MonthlyReport::temperatureColor(double temperature) {

    //t valant 10 fois la température exprimée en °C
    //t =-200 : QColor(0, 0, 0)
    //t =-150 : QColor(128, 0, 128)
    //t =-100 : QColor(128, 0, 255)
    //t = -50 : QColor(0, 0, 255)
    //t =   0 : QColor(0, 128, 255)
    //t =  50 : QColor(0, 255, 0)
    //t = 100 : QColor(255, 255, 0)
    //t = 200 : QColor(255, 128, 0)
    //t = 300 : QColor(255, 0, 0)
    //t = 350 : QColor(255, 0, 128)
    //t = 400 : QColor(255, 0, 255)
    //t = 450 : QColor(128, 0, 128)
    //t = 500 : QColor(0, 0, 0)

    int t = int(10 * temperature + 0.5 - (temperature < 0.0));

    if (t < -200) return QColor(0, 0, 0);
    if (t == -200) return QColor(0, 0, 0);
    if (t < -150) return QColor(10 + (t + 200)*108/50, 0, 10 + (t + 200)*108/50);
    if (t == -150) return QColor(128, 0, 128);
    if (t < -100) return QColor(128, 0, 138 + (t + 150)*107/50);
    if (t == -100) return QColor(128, 0, 255);
    if (t < -50) return QColor(118 + (-100 - t)*108/50, 0, 255);
    if (t == -50) return QColor(0, 0, 255);
    if (t < 0) return QColor(0, 10 + (t + 50)*108/50, 255);
    if (t == 0) return QColor(0, 128, 255);
    if (t < 50) return QColor(0, 138 + t*107/50, 245 - t*235/50);
    if (t == 50) return QColor(0, 255, 0);
    if (t < 100) return QColor(10 + (t - 50)*235/50, 255, 0);
    if (t == 100) return QColor(255, 255, 0);
    if (t < 200) return QColor(255, 245 + (100 - t)*107/100, 0);
    if (t == 200) return QColor(255, 128, 0);
    if (t < 300) return QColor(255, 118 + (200 - t)*108/100, 0);
    if (t == 300) return QColor(255, 0, 0);
    if (t < 350) return QColor(255, 0, 10 + (t - 300)*108/50);
    if (t == 350) return QColor(255, 0, 128);
    if (t < 400) return QColor(255, 0, 138 + (t - 350)*107/50);
    if (t == 400) return QColor(255, 0, 255);
    if (t < 450) return QColor(245 + (400 - t)*107/50, 0, 245 + (400 - t)*107/50);
    if (t == 450) return QColor(128, 0, 128);
    if (t < 500) return QColor(118 + (450 - t)*108/50, 0, 118 + (450 - t)*108/50);
    if (t == 500) return QColor(0, 0, 0);
    return QColor(0, 0, 0);

}

QColor MonthlyReport::humidityColor(int humidity) {
    return temperatureColor(45 - 0.6 * humidity);
}

void MonthlyReport::add1Month() {
    _date->operator=(_date->addMonths(1));
    currentMonthClickableLabel->setText(_date->toString("MMMM yyyy"));
    fillBoard();
}

void MonthlyReport::substract1Month() {
    _date->operator=(_date->addMonths(-1));
    currentMonthClickableLabel->setText(_date->toString("MMMM yyyy"));
    fillBoard();
}
