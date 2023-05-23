#include "YearlyReport.h"
#include <QDate>
#include <QDebug>

extern QString PATH_TO_PROD_DATABASE;

YearlyReport::YearlyReport()
{
    mainView = new QTableView();
    mainModel = new QStandardItemModel();

    deviceLocale = new QLocale();

    mainView->setModel(mainModel);

    dbHandler = new DatabaseHandler(PATH_TO_PROD_DATABASE);

    temperatureRadioButton = new QRadioButton("Température");
    humidityRadioButton = new QRadioButton("Humidité");
    dewPointRadioButton = new QRadioButton("Point de rosée");
    humidexRadioButton = new QRadioButton("Humidex");

    temperatureRadioButton->setChecked(true);

    interiorCheckBox = new QCheckBox("Intérieur");

    mainLayout = new QHBoxLayout();

    optionsLayout = new QVBoxLayout();
    optionsLayout->addWidget(temperatureRadioButton);
    optionsLayout->addWidget(humidityRadioButton);
    optionsLayout->addWidget(dewPointRadioButton);
    optionsLayout->addWidget(humidexRadioButton);
    optionsLayout->addWidget(interiorCheckBox);

    mainLayout->addWidget(mainView);
    mainLayout->addLayout(optionsLayout);

    setLayout(mainLayout);

    fillBoard();
}

void YearlyReport::fillBoard() {
    for (QDate date = QDate(2000, 1, 1); date <= QDate(2000, 12, 31); date = date.addDays(1)) {
        int row = date.dayOfYear() - 1;
        double txx = getMaxMaxMeasurementByDate("temperature", date.day(), date.month()).toDouble();
        double tnn = getMinMinTemperatureByDate(date.day(), date.month()).toDouble();
        double tnx = getMaxMinTemperatureByDate(date.day(), date.month()).toDouble();
        double txn = getMinMaxTemperatureByDate(date.day(), date.month()).toDouble();
        double txm = getAvgMaxTemperatureByDate(date.day(), date.month()).toDouble();
        double tnm = getAvgMinTemperatureByDate(date.day(), date.month()).toDouble();
        int txxYear = getMaxMaxMeasurementYearByDate("temperature", date.day(), date.month(), txx);
        int tnnYear = getMinMinTemperatureYearByDate(date.day(), date.month(), tnn);
        int tnxYear = getMaxMinTemperatureYearByDate(date.day(), date.month(), tnx);
        int txnYear = getMinMaxTemperatureYearByDate(date.day(), date.month(), txn);
        mainModel->setItem(row, 0, new QStandardItem(deviceLocale->toString(tnn, 'f', 1)));
        mainModel->setItem(row, 1, new QStandardItem(QString::number(tnnYear)));
        mainModel->setItem(row, 2, new QStandardItem(deviceLocale->toString(txn, 'f', 1)));
        mainModel->setItem(row, 3, new QStandardItem(QString::number(txnYear)));
        mainModel->setItem(row, 4, new QStandardItem(deviceLocale->toString(tnx, 'f', 1)));
        mainModel->setItem(row, 5, new QStandardItem(QString::number(tnxYear)));
        mainModel->setItem(row, 6, new QStandardItem(deviceLocale->toString(txx, 'f', 1)));
        mainModel->setItem(row, 7, new QStandardItem(QString::number(txxYear)));
        mainModel->setItem(row, 8, new QStandardItem(deviceLocale->toString(tnm, 'f', 1)));
        mainModel->setItem(row, 9, new QStandardItem(deviceLocale->toString(txm, 'f', 1)));
        mainModel->setVerticalHeaderItem(row, new QStandardItem(date.toString("dd/MM")));
        for (int column = 0; column < mainModel->columnCount(); column++) {
            mainModel->item(row, column)->setEditable(false);
        }
    }
}

QVariant YearlyReport::getMaxMaxMeasurementByDate(QString measurement, int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT max(max" + capitalize(measurement) + ") FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMaxMaxMeasurementYearByDate(QString measurement, int day, int month, double maxMaxMeasurement) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND max" + capitalize(measurement) + " = " + QString::number(maxMaxMeasurement)).toInt();
}

QVariant YearlyReport::getMinMinTemperatureByDate(int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT min(minTemperature) FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMinMinTemperatureYearByDate(int day, int month, double minMinTemperature) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND minTemperature = " + QString::number(minMinTemperature)).toInt();
}

QVariant YearlyReport::getMaxMinTemperatureByDate(int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT max(minTemperature) FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMaxMinTemperatureYearByDate(int day, int month, double maxMinTemperature) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND minTemperature = " + QString::number(maxMinTemperature)).toInt();
}

QVariant YearlyReport::getMinMaxTemperatureByDate(int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT min(maxTemperature) FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMinMaxTemperatureYearByDate(int day, int month, double minMaxTemperature) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND maxTemperature = " + QString::number(minMaxTemperature)).toInt();
}

QVariant YearlyReport::getAvgMaxTemperatureByDate(int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT avg(maxTemperature) FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

QVariant YearlyReport::getAvgMinTemperatureByDate(int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT avg(minTemperature) FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

QString YearlyReport::capitalize(QString s) {
    /*
     * Returns a copy of s where the first letter is in uppercase, the other ones in lowercase
     */
    return QString(s[0]).toUpper() + s.mid(1);
}

// idea: refactor all these functions providing the column (maxTemperature, minTemperature...) and the operation (min, max, avg)
// idea: add amplitude? maybe as an option?
