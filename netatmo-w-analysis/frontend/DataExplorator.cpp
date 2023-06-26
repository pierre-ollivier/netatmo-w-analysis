#include "DataExplorator.h"
#include <QLabel>

DataExplorator::DataExplorator(DatabaseHandler *dbHandler) : QWidget()
{
    _dbHandler = dbHandler;

    deviceLocale = new QLocale();

    monthComboBox = new QComboBox();
    monthComboBox->addItems({"Année complète", "Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
                             "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre"});
    monthComboBox->setCurrentIndex(QDate::currentDate().month());
    connect(monthComboBox, SIGNAL(currentIndexChanged(int)), SLOT(fillBoards()));

    mainModelMax = new QStandardItemModel(5, 2);
    mainModelMax->setHorizontalHeaderLabels(
                   QStringList({"Température maximale", "Date"}));

    mainModelMin = new QStandardItemModel(5, 2);
    mainModelMin->setHorizontalHeaderLabels(
                   QStringList({"Température minimale", "Date"}));

    mainViewMax = new QTableView();
    mainViewMax->setModel(mainModelMax);

    mainViewMin = new QTableView();
    mainViewMin->setModel(mainModelMin);

    temperatureRadioButton = new QRadioButton("Température");
    humidityRadioButton = new QRadioButton("Humidité");
    dewPointRadioButton = new QRadioButton("Point de rosée");
    humidexRadioButton = new QRadioButton("Humidex");
    pressureRadioButton = new QRadioButton("Pression");

    temperatureRadioButton->setChecked(true);

    interiorCheckBox = new QCheckBox("Intérieur");

    optionsLayout = new QHBoxLayout();
    optionsLayout->addWidget(temperatureRadioButton);
    optionsLayout->addWidget(humidityRadioButton);
    optionsLayout->addWidget(dewPointRadioButton);
    optionsLayout->addWidget(humidexRadioButton);
    optionsLayout->addWidget(pressureRadioButton);
    optionsLayout->addWidget(interiorCheckBox);

    connect(temperatureRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(humidityRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(dewPointRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(humidexRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(pressureRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(interiorCheckBox, SIGNAL(clicked()), SLOT(fillBoards()));

    layout = new QGridLayout();
    layout->addWidget(mainViewMax, 1, 1);
    layout->addWidget(mainViewMin, 1, 2);
    layout->addWidget(new QLabel("Mois : "), 2, 1);
    layout->addWidget(monthComboBox, 2, 2);
    layout->addLayout(optionsLayout, 3, 1, 1, 2);
    setLayout(layout);

    setMinimumSize(700, 400);

    fillBoards();
}

void DataExplorator::fillBoards() {
    QString monthCondition = "";
    QString operation = "max";
    QString measurementCapitalized = measurementCapitalizedFromRadioButtons();
    if (monthComboBox->currentIndex() > 0) {
        monthCondition = "WHERE month = " + QString::number(monthComboBox->currentIndex());
    }
    std::vector<QVariant> maxMeasurements = getValues(
                operation, measurementCapitalized, monthCondition, "DESC");
    std::vector<QVariant> maxMeasurementsDates = getValuesDates(
                operation, measurementCapitalized, monthCondition, "DESC");
    std::vector<QVariant> minMeasurements = getValues(
                operation, measurementCapitalized, monthCondition, "ASC");
    std::vector<QVariant> minMeasurementsDates = getValuesDates(
                operation, measurementCapitalized, monthCondition, "ASC");

    QString unitWithLeadingSpace = " °C";

    for (int i = 0; i < 5; i++) {
        mainModelMax->setItem(i, 0, new QStandardItem());
        mainModelMax->setItem(i, 1, new QStandardItem());
        mainModelMax->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMax->item(i, 0)->setText(deviceLocale->toString(maxMeasurements[i].toDouble(), 'f', 1) + unitWithLeadingSpace);
        mainModelMax->item(i, 1)->setText(maxMeasurementsDates[i].toString());
        if (i >= 1) {
            if (maxMeasurements[i] == maxMeasurements[i - 1]) {
                mainModelMax->verticalHeaderItem(i)->setText(mainModelMax->verticalHeaderItem(i - 1)->text());
            }
        }

        mainModelMin->setItem(i, 0, new QStandardItem());
        mainModelMin->setItem(i, 1, new QStandardItem());
        mainModelMin->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMin->item(i, 0)->setText(deviceLocale->toString(minMeasurements[i].toDouble(), 'f', 1) + unitWithLeadingSpace);
        mainModelMin->item(i, 1)->setText(minMeasurementsDates[i].toString());
        if (i >= 1) {
            if (minMeasurements[i] == minMeasurements[i - 1]) {
                mainModelMin->verticalHeaderItem(i)->setText(mainModelMin->verticalHeaderItem(i - 1)->text());
            }
        }
    }

    mainViewMax->resizeColumnsToContents();
    mainViewMin->resizeColumnsToContents();
}

std::vector<QVariant> DataExplorator::getValues(
        QString operation,
        QString measurementCapitalized,
        QString monthCondition,
        QString order) {

    return _dbHandler->getResultsFromDatabase(
                "SELECT " + operation + measurementCapitalized + " FROM OutdoorDailyRecords " + monthCondition + " "
                "ORDER BY " + operation + measurementCapitalized + " " + order + ", year, day LIMIT 5");
}

std::vector<QVariant> DataExplorator::getValuesDates(
        QString operation,
        QString measurementCapitalized,
        QString monthCondition,
        QString order) {

    return _dbHandler->getResultsFromDatabase(
                "SELECT date FROM OutdoorDailyRecords " + monthCondition + " "
                "ORDER BY " + operation + measurementCapitalized + " " + order + ", year, day LIMIT 5");
}

QString DataExplorator::measurementCapitalizedFromRadioButtons() {
    if (temperatureRadioButton->isChecked()) return "Temperature";
    if (humidityRadioButton->isChecked()) return "Humidity";
    if (dewPointRadioButton->isChecked()) return "DewPoint";
    if (humidexRadioButton->isChecked()) return "Humidex";
    if (pressureRadioButton->isChecked()) return "Pressure";
    return "";
}
