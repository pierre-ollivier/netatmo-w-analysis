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
//        double txx = getMaxMaxMeasurementByDate("temperature", date.day(), date.month()).toDouble();
        double txx = getMeasurementByDate("temperature", "max", "max", date.day(), date.month()).toDouble();
        double tnn = getMeasurementByDate("temperature", "min", "min", date.day(), date.month()).toDouble();
        double tnx = getMeasurementByDate("temperature", "max", "min", date.day(), date.month()).toDouble();
        double txn = getMeasurementByDate("temperature", "min", "max", date.day(), date.month()).toDouble();
        double txm = getMeasurementByDate("temperature", "avg", "max", date.day(), date.month()).toDouble();
        double tnm = getMeasurementByDate("temperature", "avg", "min", date.day(), date.month()).toDouble();
//        int txxYear = getMaxMaxMeasurementYearByDate("temperature", date.day(), date.month(), txx);
        int txxYear = getMeasurementYearByDate("temperature", "max", date.day(), date.month(), txx);
        int tnnYear = getMinMinMeasurementYearByDate("temperature", date.day(), date.month(), tnn);
        int tnxYear = getMaxMinMeasurementYearByDate("temperature", date.day(), date.month(), tnx);
        int txnYear = getMinMaxMeasurementYearByDate("temperature", date.day(), date.month(), txn);
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

QVariant YearlyReport::getMinMinMeasurementByDate(QString measurement, int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT min(min" + capitalize(measurement) + ") FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMinMinMeasurementYearByDate(QString measurement, int day, int month, double minMinMeasurement) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND min" + capitalize(measurement) + " = " + QString::number(minMinMeasurement)).toInt();
}

QVariant YearlyReport::getMaxMinMeasurementByDate(QString measurement, int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT max(min" + capitalize(measurement) + ") FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMaxMinMeasurementYearByDate(QString measurement, int day, int month, double maxMinMeasurement) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND min" + capitalize(measurement) + " = " + QString::number(maxMinMeasurement)).toInt();
}

QVariant YearlyReport::getMinMaxMeasurementByDate(QString measurement, int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT min(max" + capitalize(measurement) + ") FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMinMaxMeasurementYearByDate(QString measurement, int day, int month, double minMaxMeasurement) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND max" + capitalize(measurement) + " = " + QString::number(minMaxMeasurement)).toInt();
}

QVariant YearlyReport::getAvgMaxMeasurementByDate(QString measurement, int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT avg(max" + capitalize(measurement) + ") FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

QVariant YearlyReport::getAvgMinMeasurementByDate(QString measurement, int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT avg(min" + capitalize(measurement) + ") FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

QString YearlyReport::capitalize(QString s) {
    /*
     * Returns a copy of s where the first letter is in uppercase, the other ones in lowercase
     */
    return QString(s[0]).toUpper() + s.mid(1);
}

QVariant YearlyReport::getMeasurementByDate(QString measurement,
                                            QString operation,
                                            QString scope,
                                            int day,
                                            int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT " + operation + "(" + scope + capitalize(measurement) + ") FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMeasurementYearByDate(QString measurementType,
                                           QString scope,
                                           int day,
                                           int month,
                                           double measurementValue) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND " + scope + capitalize(measurementType) + " = " + QString::number(measurementValue)).toInt();
}

// idea: refactor all these functions providing the column (maxTemperature, minTemperature...) and the operation (min, max, avg)
// idea: add amplitude? maybe as an option?
