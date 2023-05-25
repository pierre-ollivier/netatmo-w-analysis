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
    optionsLayout->setContentsMargins(0, 350, 0, 350);

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
            mainModel->item(row, 0)->setBackground(QBrush(temperatureColor(mnn)));
            mainModel->item(row, 2)->setBackground(QBrush(temperatureColor(mxn)));
            mainModel->item(row, 4)->setBackground(QBrush(temperatureColor(mnx)));
            mainModel->item(row, 6)->setBackground(QBrush(temperatureColor(mxx)));
            mainModel->item(row, 8)->setBackground(QBrush(temperatureColor(mnm)));
            mainModel->item(row, 9)->setBackground(QBrush(temperatureColor(mxm)));
        }
        else if (unit == "%") {
            mainModel->item(row, 0)->setBackground(QBrush(humidityColor(mnn)));
            mainModel->item(row, 2)->setBackground(QBrush(humidityColor(mxn)));
            mainModel->item(row, 4)->setBackground(QBrush(humidityColor(mnx)));
            mainModel->item(row, 6)->setBackground(QBrush(humidityColor(mxx)));
            mainModel->item(row, 8)->setBackground(QBrush(humidityColor(mnm)));
            mainModel->item(row, 9)->setBackground(QBrush(humidityColor(mxm)));
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
    mainView->setFixedWidth(1290);
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
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
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

    if (interiorCheckBox->isChecked()) {
        indoorOrOutdoorCapitalized = "Indoor";
    }
    else {
        indoorOrOutdoorCapitalized = "Outdoor";
    }
    fillBoard();
}

QColor YearlyReport::temperatureColor(double temperature) {

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

QColor YearlyReport::humidityColor(int humidity) {
    return temperatureColor(45 - 0.6 * humidity);
}

// idea: add amplitude? maybe as an option?
