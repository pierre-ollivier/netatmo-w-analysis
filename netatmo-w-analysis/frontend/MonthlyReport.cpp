#include "MonthlyReport.h"
#include "../frontend/ColorUtils.h"

extern const QLocale LOCALE;
extern QString PATH_TO_PROD_DATABASE;
extern QString PATH_TO_COPY_DATABASE;
extern QDate START_DATE;

MonthlyReport::MonthlyReport() : QWidget()
{
    _date = new QDate(QDate::currentDate().addMonths(-1));
    dbHandlerCopy = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);

    yearMonthPicker = new YearMonthPicker(_date->year(), _date->month());
    connect(yearMonthPicker, SIGNAL(monthChanged(int)), SLOT(setMonth(int)));
    connect(yearMonthPicker, SIGNAL(yearChanged(int)), SLOT(setYear(int)));

    this->setGeometry(300, 40, 1020, 750);
    this->setFixedWidth(720);
    layout = new QGridLayout();

    model = new QStandardItemModel();

    view = new QTableView();
    view->setModel(model);
    view->setFixedWidth(451);

    model->setHorizontalHeaderLabels(QStringList({"T. min.", "T. max.", "T. moy."}));

    add1MonthButton = new QPushButton(">>>");
    substract1MonthButton = new QPushButton("<<<");

    connect(add1MonthButton, SIGNAL(clicked()), this, SLOT(add1Month()));
    connect(substract1MonthButton, SIGNAL(clicked()), this, SLOT(substract1Month()));

    currentMonthClickableLabel = new QPushButton(LOCALE.toString(*_date, "MMMM yyyy"));
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

    connect(temperatureRadioButton, SIGNAL(toggled(bool)), SLOT(changeMeasurement()));
    connect(humidityRadioButton, SIGNAL(toggled(bool)), SLOT(changeMeasurement()));
    connect(dewPointRadioButton, SIGNAL(toggled(bool)), SLOT(changeMeasurement()));
    connect(humidexRadioButton, SIGNAL(toggled(bool)), SLOT(changeMeasurement()));
    connect(pressureRadioButton, SIGNAL(toggled(bool)), SLOT(changeMeasurement()));
    connect(interiorCheckBox, SIGNAL(stateChanged(int)), SLOT(changeMeasurement()));

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
    for (
        QDate date = QDate(_date->year(), _date->month(), 1);
        date <= QDate(_date->year(), _date->month(), _date->daysInMonth());
        date = date.addDays(1)
        ) {

        QString measurementTypeCapitalized = QString(measurementType[0]).toUpper() + measurementType.mid(1);
        QVariant minimumMeasurement = QVariant(), maximumMeasurement = QVariant(), averageMeasurement = QVariant();

        if (date < QDate::currentDate()) {
            minimumMeasurement = dbHandlerCopy->getResultFromDatabase(
                "SELECT min" + measurementTypeCapitalized + " FROM " + indoorOrOutdoorCapitalized + "DailyRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"") + " " + extraWhereClause);
            maximumMeasurement = dbHandlerCopy->getResultFromDatabase(
                "SELECT max" + measurementTypeCapitalized + " FROM " + indoorOrOutdoorCapitalized + "DailyRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"") + " " + extraWhereClause);
            averageMeasurement = dbHandlerCopy->getResultFromDatabase(
                "SELECT avg" + measurementTypeCapitalized + " FROM " + indoorOrOutdoorCapitalized + "DailyRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"") + " " + extraWhereClause);
        }

        model->setVerticalHeaderItem(date.day() - 1, new QStandardItem(LOCALE.toString(date, "dd/MM")));

        if (minimumMeasurement.isNull())
            model->setItem(date.day() - 1, 0, new QStandardItem());
        else
            model->setItem(date.day() - 1, 0, new QStandardItem(LOCALE.toString(minimumMeasurement.toDouble(), 'f', decimals) + " " + unit));

        if (maximumMeasurement.isNull())
            model->setItem(date.day() - 1, 1, new QStandardItem());
        else
            model->setItem(date.day() - 1, 1, new QStandardItem(LOCALE.toString(maximumMeasurement.toDouble(), 'f', decimals) + " " + unit));

        if (averageMeasurement.isNull())
            model->setItem(date.day() - 1, 2, new QStandardItem());
        else
            model->setItem(date.day() - 1, 2, new QStandardItem(LOCALE.toString(averageMeasurement.toDouble(), 'f', decimals) + " " + unit));

        model->item(date.day() - 1, 0)->setEditable(false);
        model->item(date.day() - 1, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(date.day() - 1, 1)->setEditable(false);
        model->item(date.day() - 1, 1)->setTextAlignment(Qt::AlignCenter);
        model->item(date.day() - 1, 2)->setEditable(false);
        model->item(date.day() - 1, 2)->setTextAlignment(Qt::AlignCenter);

        if (unit == "°C" or unit == "") {
            model->item(date.day() - 1, 0)->setBackground(QBrush(ColorUtils::temperatureColor(minimumMeasurement)));
            model->item(date.day() - 1, 1)->setBackground(QBrush(ColorUtils::temperatureColor(maximumMeasurement)));
            model->item(date.day() - 1, 2)->setBackground(QBrush(ColorUtils::temperatureColor(averageMeasurement)));
        }
        else if (unit == "%") {
            model->item(date.day() - 1, 0)->setBackground(QBrush(ColorUtils::humidityColor(minimumMeasurement)));
            model->item(date.day() - 1, 1)->setBackground(QBrush(ColorUtils::humidityColor(maximumMeasurement)));
            model->item(date.day() - 1, 2)->setBackground(QBrush(ColorUtils::humidityColor(averageMeasurement)));
        }
        else if (unit == "hPa") {
            model->item(date.day() - 1, 0)->setBackground(QBrush(ColorUtils::pressureColor(minimumMeasurement)));
            model->item(date.day() - 1, 1)->setBackground(QBrush(ColorUtils::pressureColor(maximumMeasurement)));
            model->item(date.day() - 1, 2)->setBackground(QBrush(ColorUtils::pressureColor(averageMeasurement)));
        }

        model->setHorizontalHeaderLabels(QStringList({abbreviatedMeasurement + " min.",
                                                      abbreviatedMeasurement + " max.",
                                                      abbreviatedMeasurement + " moy."}));
    }
    // Clean the extra rows when switching from a 31-day month to a shorter one
    model->removeRows(_date->daysInMonth(), model->rowCount() - _date->daysInMonth());
}

void MonthlyReport::destroy() {
    delete this;
}

void MonthlyReport::add1Month() {
    QDate newDate = _date->addMonths(1), currentDate = QDate::currentDate();
    if (newDate.year() < currentDate.year() || (newDate.year() == currentDate.year() && newDate.month() <= currentDate.month())) {
        _date->operator=(newDate);
        currentMonthClickableLabel->setText(LOCALE.toString(newDate, "MMMM yyyy"));
        fillBoard();
        yearMonthPicker->setDate(newDate);
    }
}

void MonthlyReport::substract1Month() {
    QDate newDate = _date->addMonths(-1), minDate = START_DATE;
    if (newDate.year() > minDate.year() || (newDate.year() == minDate.year() && newDate.month() >= minDate.month())) {
        _date->operator=(_date->addMonths(-1));
        currentMonthClickableLabel->setText(LOCALE.toString(newDate, "MMMM yyyy"));
        fillBoard();
        yearMonthPicker->setDate(newDate);
    }
}

void MonthlyReport::setMonth(int month) {
    _date->setDate(_date->year(), month, _date->day());
    currentMonthClickableLabel->setText(LOCALE.toString(*_date, "MMMM yyyy"));
    fillBoard();
}

void MonthlyReport::setYear(int year) {
    _date->setDate(year, _date->month(), _date->day());
    currentMonthClickableLabel->setText(LOCALE.toString(*_date, "MMMM yyyy"));
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
        extraWhereClause = "";
    }
    else if (humidityRadioButton->isChecked()) {
        measurementType = "humidity";
        abbreviatedMeasurement = "HR";
        unit = "%";
        decimals = 0;
        extraWhereClause = "";
    }
    else if (dewPointRadioButton->isChecked()) {
        measurementType = "dewPoint";
        abbreviatedMeasurement = "PdR";
        unit = "°C";
        decimals = 1;
        extraWhereClause = "";
    }
    else if (humidexRadioButton->isChecked()) {
        measurementType = "humidex";
        abbreviatedMeasurement = "Hx";
        unit = "";
        decimals = 1;
        extraWhereClause = "";
    }
    else if (pressureRadioButton->isChecked()) {
        measurementType = "pressure";
        abbreviatedMeasurement = "P.";
        unit = "hPa";
        decimals = 1;
        indoorOrOutdoorCapitalized = "Indoor";
        extraWhereClause = "AND minPressure > 950";
    }

    fillBoard();
}
