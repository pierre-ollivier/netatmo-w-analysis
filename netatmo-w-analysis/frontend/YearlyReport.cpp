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

    connect(temperatureRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(humidityRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(dewPointRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(humidexRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeMeasurement()));
    connect(interiorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeMeasurement()));

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
    mainModel->clear();
    for (QDate date = QDate(2000, 1, 1); date <= QDate(2000, 12, 31); date = date.addDays(1)) {
        int row = date.dayOfYear() - 1;
        double txx = getMeasurementByDate(measurementType, "max", "max", date.day(), date.month()).toDouble();
        double tnn = getMeasurementByDate(measurementType, "min", "min", date.day(), date.month()).toDouble();
        double tnx = getMeasurementByDate(measurementType, "max", "min", date.day(), date.month()).toDouble();
        double txn = getMeasurementByDate(measurementType, "min", "max", date.day(), date.month()).toDouble();
        double txm = getMeasurementByDate(measurementType, "avg", "max", date.day(), date.month()).toDouble();
        double tnm = getMeasurementByDate(measurementType, "avg", "min", date.day(), date.month()).toDouble();
        int txxYear = getMeasurementYearByDate(measurementType, "max", date.day(), date.month(), txx);
        int tnnYear = getMeasurementYearByDate(measurementType, "min", date.day(), date.month(), tnn);
        int tnxYear = getMeasurementYearByDate(measurementType, "min", date.day(), date.month(), tnx);
        int txnYear = getMeasurementYearByDate(measurementType, "max", date.day(), date.month(), txn);
        mainModel->setItem(row, 0, new QStandardItem(deviceLocale->toString(tnn, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 1, new QStandardItem(QString::number(tnnYear)));
        mainModel->setItem(row, 2, new QStandardItem(deviceLocale->toString(txn, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 3, new QStandardItem(QString::number(txnYear)));
        mainModel->setItem(row, 4, new QStandardItem(deviceLocale->toString(tnx, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 5, new QStandardItem(QString::number(tnxYear)));
        mainModel->setItem(row, 6, new QStandardItem(deviceLocale->toString(txx, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 7, new QStandardItem(QString::number(txxYear)));
        mainModel->setItem(row, 8, new QStandardItem(deviceLocale->toString(tnm, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 9, new QStandardItem(deviceLocale->toString(txm, 'f', decimals) + " " + unit));
        mainModel->setVerticalHeaderItem(row, new QStandardItem(date.toString("dd/MM")));
        for (int column = 0; column < mainModel->columnCount(); column++) {
            mainModel->item(row, column)->setEditable(false);
        }
    }
    mainModel->setHorizontalHeaderLabels(
                QStringList({
                                legends[0],
                                "Année",
                                legends[1],
                                "Année",
                                legends[2],
                                "Année",
                                legends[3],
                                "Année",
                                legends[4],
                                legends[5]
                            }));
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

void YearlyReport::changeMeasurement() {
    if (temperatureRadioButton->isChecked()) {
        measurementType = "temperature";
        legends = QStringList({
            "Tnn",
            "Txn",
            "Tnx",
            "Txx",
            "Température minimale moyenne",
            "Température maximale moyenne"
        });
        unit = "°C";
        decimals = 1;
    }
    else if (humidityRadioButton->isChecked()) {
        measurementType = "humidity";
        legends = QStringList({
            "HRnn",
            "HRxn",
            "HRnx",
            "HRxx",
            "Humidité minimale moyenne",
            "Humidité maximale moyenne"
        });
        unit = "%";
        decimals = 0;
    }
    else if (dewPointRadioButton->isChecked()) {
        measurementType = "dewPoint";
        legends = QStringList({
            "Tdnn",
            "Tdxn",
            "Tdnx",
            "Tdxx",
            "Point de rosée minimal moyen",
            "Point de rosée maximal moyen"
        });
        unit = "°C";
        decimals = 1;
    }
    else {
        measurementType = "humidex";
        legends = QStringList({
            "Hxnn",
            "Hxxn",
            "Hxnx",
            "Hxxx",
            "Humidex minimal moyen",
            "Humidex maximal moyen"
        });
        unit = "";
        decimals = 1;
    }
    fillBoard();
}

// idea: add amplitude? maybe as an option?
