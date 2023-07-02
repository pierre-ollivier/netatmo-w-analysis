#include "YearlyReport.h"
#include <QDate>
#include <QDebug>
#include "../frontend/ColorUtils.h"

extern QString PATH_TO_PROD_DATABASE;
extern QString PATH_TO_COPY_DATABASE;

YearlyReport::YearlyReport()
{
    mainView = new QTableView();
    mainModel = new QStandardItemModel();

    deviceLocale = new QLocale();

    mainView->setModel(mainModel);

    dbHandler = new DatabaseHandler(PATH_TO_COPY_DATABASE);

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

    mainLayout = new QHBoxLayout();

    optionsLayout = new QVBoxLayout();
    optionsLayout->addWidget(temperatureRadioButton);
    optionsLayout->addWidget(humidityRadioButton);
    optionsLayout->addWidget(dewPointRadioButton);
    optionsLayout->addWidget(humidexRadioButton);
    optionsLayout->addWidget(pressureRadioButton);
    optionsLayout->addWidget(interiorCheckBox);
    optionsLayout->setContentsMargins(0, 380, 0, 380);

    mainLayout->addWidget(mainView);
    mainLayout->addLayout(optionsLayout);

    setLayout(mainLayout);
    setFixedSize(1480, 1000);

    fillBoard();
}

void YearlyReport::fillBoard() {
    mainModel->clear();
    for (QDate date = QDate(2000, 1, 1); date <= QDate(2000, 12, 31); date = date.addDays(1)) {
        int row = date.dayOfYear() - 1;
        double mxx = getMeasurementByDate(measurementType, "max", "max", date.day(), date.month()).toDouble();
        double mnn = getMeasurementByDate(measurementType, "min", "min", date.day(), date.month()).toDouble();
        double mnx = getMeasurementByDate(measurementType, "max", "min", date.day(), date.month()).toDouble();
        double mxn = getMeasurementByDate(measurementType, "min", "max", date.day(), date.month()).toDouble();
        double mxm = getMeasurementByDate(measurementType, "avg", "max", date.day(), date.month()).toDouble();
        double mnm = getMeasurementByDate(measurementType, "avg", "min", date.day(), date.month()).toDouble();
        int mxxYear = getMeasurementYearByDate(measurementType, "max", date.day(), date.month(), mxx);
        int mnnYear = getMeasurementYearByDate(measurementType, "min", date.day(), date.month(), mnn);
        int mnxYear = getMeasurementYearByDate(measurementType, "min", date.day(), date.month(), mnx);
        int mxnYear = getMeasurementYearByDate(measurementType, "max", date.day(), date.month(), mxn);

        mainModel->setItem(row, 0, new QStandardItem(deviceLocale->toString(mnn, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 1, new QStandardItem(QString::number(mnnYear)));
        mainModel->setItem(row, 2, new QStandardItem(deviceLocale->toString(mxn, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 3, new QStandardItem(QString::number(mxnYear)));
        mainModel->setItem(row, 4, new QStandardItem(deviceLocale->toString(mnx, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 5, new QStandardItem(QString::number(mnxYear)));
        mainModel->setItem(row, 6, new QStandardItem(deviceLocale->toString(mxx, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 7, new QStandardItem(QString::number(mxxYear)));
        mainModel->setItem(row, 8, new QStandardItem(deviceLocale->toString(mnm, 'f', decimals) + " " + unit));
        mainModel->setItem(row, 9, new QStandardItem(deviceLocale->toString(mxm, 'f', decimals) + " " + unit));
        mainModel->setVerticalHeaderItem(row, new QStandardItem(date.toString("dd/MM")));

        for (int column = 0; column < mainModel->columnCount(); column++) {
            mainModel->item(row, column)->setEditable(false);
        }

        if (unit == "°C" or unit == "") {
            mainModel->item(row, 0)->setBackground(QBrush(ColorUtils::temperatureColor(mnn)));
            mainModel->item(row, 2)->setBackground(QBrush(ColorUtils::temperatureColor(mxn)));
            mainModel->item(row, 4)->setBackground(QBrush(ColorUtils::temperatureColor(mnx)));
            mainModel->item(row, 6)->setBackground(QBrush(ColorUtils::temperatureColor(mxx)));
            mainModel->item(row, 8)->setBackground(QBrush(ColorUtils::temperatureColor(mnm)));
            mainModel->item(row, 9)->setBackground(QBrush(ColorUtils::temperatureColor(mxm)));
        }
        else if (unit == "%") {
            mainModel->item(row, 0)->setBackground(QBrush(ColorUtils::humidityColor(mnn)));
            mainModel->item(row, 2)->setBackground(QBrush(ColorUtils::humidityColor(mxn)));
            mainModel->item(row, 4)->setBackground(QBrush(ColorUtils::humidityColor(mnx)));
            mainModel->item(row, 6)->setBackground(QBrush(ColorUtils::humidityColor(mxx)));
            mainModel->item(row, 8)->setBackground(QBrush(ColorUtils::humidityColor(mnm)));
            mainModel->item(row, 9)->setBackground(QBrush(ColorUtils::humidityColor(mxm)));
        }
        else if (unit == "hPa") {
            mainModel->item(row, 0)->setBackground(QBrush(ColorUtils::pressureColor(mnn)));
            mainModel->item(row, 2)->setBackground(QBrush(ColorUtils::pressureColor(mxn)));
            mainModel->item(row, 4)->setBackground(QBrush(ColorUtils::pressureColor(mnx)));
            mainModel->item(row, 6)->setBackground(QBrush(ColorUtils::pressureColor(mxx)));
            mainModel->item(row, 8)->setBackground(QBrush(ColorUtils::pressureColor(mnm)));
            mainModel->item(row, 9)->setBackground(QBrush(ColorUtils::pressureColor(mxm)));
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

    mainView->setColumnWidth(1, 60);
    mainView->setColumnWidth(3, 60);
    mainView->setColumnWidth(5, 60);
    mainView->setColumnWidth(7, 60);
    mainView->setColumnWidth(8, 240);
    mainView->setColumnWidth(9, 240);
    mainView->setFixedWidth(1296);
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
                "SELECT " + operation + "(" + scope + capitalize(measurement) + ") "
                "FROM " + indoorOrOutdoorCapitalized + "DailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month) + " " + extraWhereClause);
}

int YearlyReport::getMeasurementYearByDate(QString measurementType,
                                           QString scope,
                                           int day,
                                           int month,
                                           double measurementValue) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM " + indoorOrOutdoorCapitalized + "DailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND " + scope + capitalize(measurementType) + " = "
                + QString::number(measurementValue, 'f', 6) + " " + extraWhereClause).toInt();
}

void YearlyReport::changeMeasurement() {
    if (interiorCheckBox->isChecked()) {
        indoorOrOutdoorCapitalized = "Indoor";
    }
    else {
        indoorOrOutdoorCapitalized = "Outdoor";
    }

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
        extraWhereClause = "";
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
        extraWhereClause = "";
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
        extraWhereClause = "";
    }
    else if (humidexRadioButton->isChecked()) {
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
        extraWhereClause = "";
    }
    else if (pressureRadioButton->isChecked()) {
        measurementType = "pressure";
        legends = QStringList({
            "Pnn",
            "Pxn",
            "Pnx",
            "Pxx",
            "Pression minimale moyenne",
            "Pression maximale moyenne"
        });
        unit = "hPa";
        decimals = 1;
        indoorOrOutdoorCapitalized = "Indoor";
        extraWhereClause = "AND maxPressure > 950 AND minPressure > 950";
    }

    fillBoard();
}

// idea: add amplitude? maybe as an option?
