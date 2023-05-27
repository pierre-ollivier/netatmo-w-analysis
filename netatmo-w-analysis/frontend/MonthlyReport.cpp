#include "MonthlyReport.h"

extern QString PATH_TO_PROD_DATABASE;

MonthlyReport::MonthlyReport() : QWidget()
{
    _date = new QDate(QDate::currentDate().addMonths(-1));
    dbHandler = new DatabaseHandler(PATH_TO_PROD_DATABASE);
    deviceLocale = new QLocale();

    yearMonthPicker = new YearMonthPicker(_date->year(), _date->month());
    connect(yearMonthPicker, SIGNAL(monthChanged(int)), SLOT(setMonth(int)));
    connect(yearMonthPicker, SIGNAL(yearChanged(int)), SLOT(setYear(int)));

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
    currentMonthClickableLabel->setFont(QFont("Arial", 14));
    connect(currentMonthClickableLabel, SIGNAL(clicked()), yearMonthPicker, SLOT(show()));

    temperatureRadioButton = new QRadioButton("Température");
    humidityRadioButton = new QRadioButton("Humidité");
    dewPointRadioButton = new QRadioButton("Point de rosée");
    humidexRadioButton = new QRadioButton("Humidex");
    pressureRadioButton = new QRadioButton("Pression");

    temperatureRadioButton->setChecked(true);

    interiorCheckBox = new QCheckBox("Intérieur");

    connect(temperatureRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(humidityRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(dewPointRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(humidexRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(pressureRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(interiorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeMeasurement()));

    buttonsLayout = new QVBoxLayout();
    buttonsLayout->addWidget(temperatureRadioButton);
    buttonsLayout->addWidget(humidityRadioButton);
    buttonsLayout->addWidget(dewPointRadioButton);
    buttonsLayout->addWidget(humidexRadioButton);
    buttonsLayout->addWidget(pressureRadioButton);
    buttonsLayout->addWidget(interiorCheckBox);

    layout->addWidget(add1MonthButton, 0, 2);
    layout->addWidget(substract1MonthButton, 0, 0);
    layout->addWidget(currentMonthClickableLabel, 0, 1);
    layout->addWidget(view, 1, 0, 3, 2);
    layout->addLayout(buttonsLayout, 1, 2);

    setLayout(layout);
    fillBoard();

}


void MonthlyReport::fillBoard() {
    for (int day = 1; day <= _date->daysInMonth(); day++) {
        QDate date = QDate(_date->year(), _date->month(), day);
        QString measurementTypeCapitalized = QString(measurementType[0]).toUpper() + measurementType.mid(1);
        double minimumMeasurement = dbHandler->getResultFromDatabase(
                    "SELECT min" + measurementTypeCapitalized + " FROM " + indoorOrOutdoorCapitalized + "DailyRecords WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
        double maximumMeasurement = dbHandler->getResultFromDatabase(
                    "SELECT max" + measurementTypeCapitalized + " FROM " + indoorOrOutdoorCapitalized + "DailyRecords WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
        double averageMeasurement = dbHandler->getResultFromDatabase(
                    "SELECT avg" + measurementTypeCapitalized + " FROM " + indoorOrOutdoorCapitalized + "DailyRecords WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();

        model->setVerticalHeaderItem(day - 1, new QStandardItem(date.toString("dd/MM")));
        model->setItem(day - 1, 0, new QStandardItem(deviceLocale->toString(minimumMeasurement, 'f', decimals) + " " + unit));

        model->setVerticalHeaderItem(day - 1, new QStandardItem(date.toString("dd/MM")));
        model->setItem(day - 1, 1, new QStandardItem(deviceLocale->toString(maximumMeasurement, 'f', decimals) + " " + unit));


        model->setVerticalHeaderItem(day - 1, new QStandardItem(date.toString("dd/MM")));
        model->setItem(day - 1, 2, new QStandardItem(deviceLocale->toString(averageMeasurement, 'f', decimals) + " " + unit));

        model->item(day - 1, 0)->setEditable(false);
        model->item(day - 1, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(day - 1, 1)->setEditable(false);
        model->item(day - 1, 1)->setTextAlignment(Qt::AlignCenter);
        model->item(day - 1, 2)->setEditable(false);
        model->item(day - 1, 2)->setTextAlignment(Qt::AlignCenter);

        if (unit == "°C" or unit == "") {
            model->item(day - 1, 0)->setBackground(QBrush(temperatureColor(minimumMeasurement)));
            model->item(day - 1, 1)->setBackground(QBrush(temperatureColor(maximumMeasurement)));
            model->item(day - 1, 2)->setBackground(QBrush(temperatureColor(averageMeasurement)));
        }
        else if (unit == "%") {
            model->item(day - 1, 0)->setBackground(QBrush(humidityColor(minimumMeasurement)));
            model->item(day - 1, 1)->setBackground(QBrush(humidityColor(maximumMeasurement)));
            model->item(day - 1, 2)->setBackground(QBrush(humidityColor(averageMeasurement)));
        }
        else if (unit == "hPa") {
            model->item(day - 1, 0)->setBackground(QBrush(pressureColor(minimumMeasurement)));
            model->item(day - 1, 1)->setBackground(QBrush(pressureColor(maximumMeasurement)));
            model->item(day - 1, 2)->setBackground(QBrush(pressureColor(averageMeasurement)));
        }

        model->setHorizontalHeaderLabels(QStringList({abbreviatedMeasurement + " min.",
                                                      abbreviatedMeasurement + " max.",
                                                      abbreviatedMeasurement + " moy."}));
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

QColor MonthlyReport::pressureColor(double pressure) {
    return temperatureColor(-15 + (pressure - 960) * 60 / 126);
}

void MonthlyReport::add1Month() {
    QDate newDate = _date->addMonths(1), currentDate = QDate::currentDate();
    if (newDate.year() < currentDate.year() || (newDate.year() == currentDate.year() && newDate.month() <= currentDate.month())) {
        _date->operator=(newDate);
        currentMonthClickableLabel->setText(_date->toString("MMMM yyyy"));
        fillBoard();
    }
}

void MonthlyReport::substract1Month() {
    QDate newDate = _date->addMonths(-1), minDate = QDate(2019, 10, 5);
    if (newDate.year() > minDate.year() || (newDate.year() == minDate.year() && newDate.month() >= minDate.month())) {
        _date->operator=(_date->addMonths(-1));
        currentMonthClickableLabel->setText(_date->toString("MMMM yyyy"));
        fillBoard();
    }
}

void MonthlyReport::setMonth(int month) {
    _date->setDate(_date->year(), month, _date->day());
    currentMonthClickableLabel->setText(_date->toString("MMMM yyyy"));
    fillBoard();
}

void MonthlyReport::setYear(int year) {
    _date->setDate(year, _date->month(), _date->day());
    currentMonthClickableLabel->setText(_date->toString("MMMM yyyy"));
    fillBoard();
}

void MonthlyReport::changeMeasurement() { 
    if (interiorCheckBox->isChecked()) {
        indoorOrOutdoorCapitalized = "Indoor";
    }
    else {
        indoorOrOutdoorCapitalized = "Outdoor";
    }

    if (temperatureRadioButton->isChecked()) {
        measurementType = "temperature";
        abbreviatedMeasurement = "T.";
        unit = "°C";
        decimals = 1;
    }
    else if (humidityRadioButton->isChecked()) {
        measurementType = "humidity";
        abbreviatedMeasurement = "HR";
        unit = "%";
        decimals = 0;
    }
    else if (dewPointRadioButton->isChecked()) {
        measurementType = "dewPoint";
        abbreviatedMeasurement = "PdR";
        unit = "°C";
        decimals = 1;
    }
    else if (humidexRadioButton->isChecked()) {
        measurementType = "humidex";
        abbreviatedMeasurement = "Hx";
        unit = "";
        decimals = 1;
    }
    else if (pressureRadioButton->isChecked()) {
        measurementType = "pressure";
        abbreviatedMeasurement = "P.";
        unit = "hPa";
        decimals = 1;
        indoorOrOutdoorCapitalized = "Indoor";
    }

    fillBoard();
}
