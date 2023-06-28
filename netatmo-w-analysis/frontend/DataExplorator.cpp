#include "DataExplorator.h"
#include <QDebug>
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

    measurementsGroupBox = new QGroupBox();
    temperatureRadioButton = new QRadioButton("Température");
    humidityRadioButton = new QRadioButton("Humidité");
    dewPointRadioButton = new QRadioButton("Point de rosée");
    humidexRadioButton = new QRadioButton("Humidex");
    pressureRadioButton = new QRadioButton("Pression");

    temperatureRadioButton->setChecked(true);

    operationsGroupBox = new QGroupBox();
    maximumRadioButton = new QRadioButton("Maximum");
    minimumRadioButton = new QRadioButton("Minimum");
    averageRadioButton = new QRadioButton("Moyenne");
    differenceRadioButton = new QRadioButton("Amplitude");

    maximumRadioButton->setChecked(true);

    interiorCheckBox = new QCheckBox("Intérieur");

    moreResultsButton = new QPushButton("Plus...");
    lessResultsButton = new QPushButton("Moins...");

    connect(moreResultsButton, SIGNAL(clicked()), SLOT(displayMoreResults()));
    connect(lessResultsButton, SIGNAL(clicked()), SLOT(displayLessResults()));

    optionsLayout = new QHBoxLayout();
    optionsLayout->addWidget(temperatureRadioButton);
    optionsLayout->addWidget(humidityRadioButton);
    optionsLayout->addWidget(dewPointRadioButton);
    optionsLayout->addWidget(humidexRadioButton);
    optionsLayout->addWidget(pressureRadioButton);
    optionsLayout->addWidget(interiorCheckBox);

    measurementsGroupBox->setLayout(optionsLayout);

    operationsLayout = new QVBoxLayout();
    operationsLayout->addWidget(maximumRadioButton);
    operationsLayout->addWidget(minimumRadioButton);
    operationsLayout->addWidget(averageRadioButton);
    operationsLayout->addWidget(differenceRadioButton);

    operationsGroupBox->setLayout(operationsLayout);

    connect(temperatureRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(humidityRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(dewPointRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(humidexRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(pressureRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(maximumRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(minimumRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(averageRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(differenceRadioButton, SIGNAL(clicked()), SLOT(fillBoards()));
    connect(interiorCheckBox, SIGNAL(clicked()), SLOT(fillBoards()));

    layout = new QGridLayout();
    layout->addWidget(mainViewMax, 1, 1);
    layout->addWidget(mainViewMin, 1, 3);
    layout->addWidget(moreResultsButton, 2, 1);
    layout->addWidget(lessResultsButton, 2, 3);
    layout->addWidget(new QLabel("Mois : "), 3, 1);
    layout->addWidget(monthComboBox, 3, 2);
    layout->addWidget(measurementsGroupBox, 4, 1, 1, 2);
    layout->addWidget(operationsGroupBox, 3, 3, 3, 1);
    setLayout(layout);

    setMinimumSize(900, 400);

    fillBoards();
}

void DataExplorator::fillBoards() {
    QString databaseName = databaseFromCheckBox();
    QString monthCondition = "";
    QString operation = operationFromRadioButtons();
    QString measurementCapitalized = measurementCapitalizedFromRadioButtons();

    if (monthComboBox->currentIndex() > 0 && pressureRadioButton->isChecked()) {
        monthCondition = "WHERE month = " + QString::number(monthComboBox->currentIndex()) + " "
                         "AND minPressure > 900";
    }
    else if (monthComboBox->currentIndex() > 0) {
        monthCondition = "WHERE month = " + QString::number(monthComboBox->currentIndex());
    }
    else if (pressureRadioButton->isChecked()) {
        monthCondition = "WHERE minPressure > 900";
    }

    std::vector<QVariant> maxMeasurements = getValues(
                databaseName, operation, measurementCapitalized, monthCondition, "DESC");
    std::vector<QVariant> maxMeasurementsDates = getValuesDates(
                databaseName, operation, measurementCapitalized, monthCondition, "DESC");
    std::vector<QVariant> minMeasurements = getValues(
                databaseName, operation, measurementCapitalized, monthCondition, "ASC");
    std::vector<QVariant> minMeasurementsDates = getValuesDates(
                databaseName, operation, measurementCapitalized, monthCondition, "ASC");

    QString unitWithLeadingSpace = unitWithLeadingSpaceFromRadioButtons();
    int decimalCount = humidityRadioButton->isChecked() ? 0 : 1;

    for (int i = 0; i < numberOfResults; i++) {
        mainModelMax->setItem(i, 0, new QStandardItem());
        mainModelMax->setItem(i, 1, new QStandardItem());
        mainModelMax->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMax->item(i, 0)->setText(
                    deviceLocale->toString(
                        maxMeasurements[i].toDouble(), 'f', decimalCount) + unitWithLeadingSpace);
        mainModelMax->item(i, 1)->setText(maxMeasurementsDates[i].toString());
        if (i >= 1) {
            if (maxMeasurements[i] == maxMeasurements[i - 1]) {
                mainModelMax->verticalHeaderItem(i)->setText(mainModelMax->verticalHeaderItem(i - 1)->text());
            }
        }

        mainModelMin->setItem(i, 0, new QStandardItem());
        mainModelMin->setItem(i, 1, new QStandardItem());
        mainModelMin->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMin->item(i, 0)->setText(
                    deviceLocale->toString(
                        minMeasurements[i].toDouble(), 'f', decimalCount) + unitWithLeadingSpace);
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
        QString databaseName,
        QString operation,
        QString measurementCapitalized,
        QString monthCondition,
        QString order,
        int limit) {

    if (limit == 0) limit = numberOfResults;
    if (operation != "diff") {
        return _dbHandler->getResultsFromDatabase(
                    "SELECT " + operation + measurementCapitalized + " "
                    "FROM " + databaseName + " " + monthCondition + " "
                    "ORDER BY " + operation + measurementCapitalized + " "
                    + order + ", year, month, day LIMIT " + QString::number(limit));
    }

    return _dbHandler->getResultsFromDatabase(
                "SELECT (max" + measurementCapitalized + " - min" + measurementCapitalized + ") "
                "FROM " + databaseName + " " + monthCondition + " "
                "ORDER BY (max" + measurementCapitalized + " - min" + measurementCapitalized + ") "
                + order + ", year, month, day LIMIT " + QString::number(limit));

}

std::vector<QVariant> DataExplorator::getValuesDates(
        QString databaseName,
        QString operation,
        QString measurementCapitalized,
        QString monthCondition,
        QString order,
        int limit) {

    if (limit == 0) limit = numberOfResults;
    if (operation != "diff") {
        return _dbHandler->getResultsFromDatabase(
                    "SELECT date FROM " + databaseName + " " + monthCondition + " "
                    "ORDER BY " + operation + measurementCapitalized + " " + order + ", "
                    "year ASC, month ASC, day ASC LIMIT " + QString::number(limit));
    }

    return _dbHandler->getResultsFromDatabase(
                "SELECT date FROM " + databaseName + " " + monthCondition + " "
                "ORDER BY (max" + measurementCapitalized + " - min" + measurementCapitalized + ") "
                + order + ", year ASC, month ASC, day ASC LIMIT " + QString::number(limit));
}

QString DataExplorator::measurementCapitalizedFromRadioButtons() {
    if (temperatureRadioButton->isChecked()) return "Temperature";
    if (humidityRadioButton->isChecked()) return "Humidity";
    if (dewPointRadioButton->isChecked()) return "DewPoint";
    if (humidexRadioButton->isChecked()) return "Humidex";
    if (pressureRadioButton->isChecked()) return "Pressure";
    return "";
}

QString DataExplorator::operationFromRadioButtons() {
    if (maximumRadioButton->isChecked()) return "max";
    if (minimumRadioButton->isChecked()) return "min";
    if (averageRadioButton->isChecked()) return "avg";
    if (differenceRadioButton->isChecked()) return "diff";
    return "";
}

QString DataExplorator::unitWithLeadingSpaceFromRadioButtons() {
    if (temperatureRadioButton->isChecked()) return " °C";
    if (humidityRadioButton->isChecked()) return " %";
    if (dewPointRadioButton->isChecked()) return " °C";
    if (humidexRadioButton->isChecked()) return "";
    if (pressureRadioButton->isChecked()) return " hPa";
    return "";
}

QString DataExplorator::databaseFromCheckBox() {
    if (interiorCheckBox->isChecked() || pressureRadioButton->isChecked()) {
        return "IndoorDailyRecords";
    }
    return "OutdoorDailyRecords";
}

void DataExplorator::displayMoreResults() {
    numberOfResults += 5;
    for (int i = 0; i < 5; i++) {
        mainModelMax->appendRow(new QStandardItem());
        mainModelMin->appendRow(new QStandardItem());
    }
    fillBoards();
}

void DataExplorator::displayLessResults() {

}
