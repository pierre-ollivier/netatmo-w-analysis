#include "DataExplorator.h"
#include <QDebug>
#include <QLabel>
#include "../frontend/CustomItemDelegate.h"
#include "../frontend/ColorUtils.h"

DataExplorator::DataExplorator(DatabaseHandler *dbHandler) : QWidget()
{
    _dbHandler = dbHandler;

    deviceLocale = new QLocale();

    monthComboBox = new QComboBox();
    monthComboBox->addItems({"Janvier", "Février", "Mars", "Avril", "Mai", "Juin",
                             "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre",
                             "Printemps", "Été", "Automne", "Hiver", "Année complète"});
    monthComboBox->setCurrentIndex(QDate::currentDate().month() - 1);
    connect(monthComboBox, SIGNAL(currentIndexChanged(int)), SLOT(changeDisplayMonth()));

    mainModelMax = new QStandardItemModel(5, 2);
    mainModelMax->setHorizontalHeaderLabels(
                   QStringList({"Max. de la température max.", "Date"}));

    mainModelMin = new QStandardItemModel(5, 2);
    mainModelMin->setHorizontalHeaderLabels(
                   QStringList({"Min. de la température max.", "Date"}));

    mainViewMax = new QTableView();
    mainViewMax->setModel(mainModelMax);
    mainViewMax->setFixedWidth(370);
    mainViewMax->setColumnWidth(0, 220);
    mainViewMax->setItemDelegate(new CustomItemDelegate());

    mainViewMin = new QTableView();
    mainViewMin->setModel(mainModelMin);
    mainViewMin->setFixedWidth(370);
    mainViewMin->setColumnWidth(0, 220);
    mainViewMin->setItemDelegate(new CustomItemDelegate());

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

    customQueryGroupBox = new QGroupBox();
    customQueryLineEdit = new QLineEdit();
    customQueryLayout = new QGridLayout();
    sendQueryButton = new QPushButton("OK");

    queryParamsSelected = new QRadioButton(this);
    customQuerySelected = new QRadioButton(this);

    queryParamsSelected->setFixedWidth(20);
    customQuerySelected->setFixedWidth(20);

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

    operationsLayout = new QGridLayout();
    operationsLayout->addWidget(maximumRadioButton, 0, 0);
    operationsLayout->addWidget(minimumRadioButton, 1, 0);
    operationsLayout->addWidget(averageRadioButton, 0, 1);
    operationsLayout->addWidget(differenceRadioButton, 1, 1);

    operationsGroupBox->setLayout(operationsLayout);

    customQueryLayout = new QGridLayout();
    customQueryLayout->addWidget(new QLabel("Ou bien entrez une requête :"), 0, 0, 1, 12);
    customQueryLayout->addWidget(customQueryLineEdit, 1, 0, 1, 11);
    customQueryLayout->addWidget(sendQueryButton, 1, 11);

    customQueryGroupBox->setLayout(customQueryLayout);

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

    connect(sendQueryButton, SIGNAL(clicked()), SLOT(sendRequest()));

    connect(queryParamsSelected, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(customQuerySelected, SIGNAL(clicked()), SLOT(selectCustomQuery()));

    layout = new QGridLayout();
    layout->addWidget(mainViewMax, 1, 0, 1, 2);
    layout->addWidget(mainViewMin, 1, 4);
    layout->addWidget(moreResultsButton, 2, 0, 1, 2);
    layout->addWidget(lessResultsButton, 2, 4);
    layout->addWidget(new QLabel("Période : "), 2, 2);
    layout->addWidget(monthComboBox, 2, 3);
    layout->addWidget(queryParamsSelected, 3, 0);
    layout->addWidget(measurementsGroupBox, 3, 1, 1, 3);
    layout->addWidget(operationsGroupBox, 3, 4);
    layout->addWidget(customQuerySelected, 4, 0);
    layout->addWidget(customQueryGroupBox, 4, 1, 1, 4);
    setLayout(layout);

    setMinimumSize(830, 810);

    fillBoards();
}

void DataExplorator::fillBoards() {
    lastOperationWasFromCustomQuery = false;
    QString databaseName = databaseFromCheckBox();
    QString monthCondition = "";
    QString operation = operationFromRadioButtons();
    QString measurementCapitalized = measurementCapitalizedFromRadioButtons();
    QString condition = conditionFromWidgets();

    std::vector<QVariant> maxMeasurements = getValues(
                databaseName, operation, measurementCapitalized, condition, "DESC");
    std::vector<QVariant> maxMeasurementsDates = getValuesDates(
                databaseName, operation, measurementCapitalized, condition, "DESC");
    std::vector<QVariant> minMeasurements = getValues(
                databaseName, operation, measurementCapitalized, condition, "ASC");
    std::vector<QVariant> minMeasurementsDates = getValuesDates(
                databaseName, operation, measurementCapitalized, condition, "ASC");

    QString unitWithLeadingSpace = unitWithLeadingSpaceFromRadioButtons();
    int decimalCount = humidityRadioButton->isChecked() ? 0 : 1;
    displayHeadersFromRadioButtons();

    for (int i = 0; i < int(minMeasurements.size()); i++) {

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

        // set colors

        if (humidityRadioButton->isChecked()) {
            mainModelMax->item(i, 0)->setBackground(QBrush(ColorUtils::humidityColor(maxMeasurements[i])));
            mainModelMin->item(i, 0)->setBackground(QBrush(ColorUtils::humidityColor(minMeasurements[i])));
        }
        else if (pressureRadioButton->isChecked()) {
            mainModelMax->item(i, 0)->setBackground(QBrush(ColorUtils::pressureColor(maxMeasurements[i])));
            mainModelMin->item(i, 0)->setBackground(QBrush(ColorUtils::pressureColor(minMeasurements[i])));
        }
        else {
            mainModelMax->item(i, 0)->setBackground(QBrush(ColorUtils::temperatureColor(maxMeasurements[i])));
            mainModelMin->item(i, 0)->setBackground(QBrush(ColorUtils::temperatureColor(minMeasurements[i])));
        }
    }

    mainViewMax->resizeColumnsToContents();
    mainViewMin->resizeColumnsToContents();
}

void DataExplorator::fillBoards(QString query) {
    lastOperationWasFromCustomQuery = true;
    std::vector<QVariant> data = getValues(query, numberOfResults);
    for (int i = 0; i < int(data.size()); i++) {
        mainModelMax->setItem(i, 0, new QStandardItem());
        mainModelMax->setItem(i, 1, new QStandardItem());
        mainModelMax->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMax->item(i, 0)->setText(
                    deviceLocale->toString(
                        data[i].toDouble(), 'f', 3));
        mainModelMax->item(i, 1)->setText(data[i].toString());
        if (i >= 1) {
            if (data[i] == data[i - 1]) {
                mainModelMax->verticalHeaderItem(i)->setText(mainModelMax->verticalHeaderItem(i - 1)->text());
            }
        }

        mainModelMin->setItem(i, 0, new QStandardItem());
        mainModelMin->setItem(i, 1, new QStandardItem());
        mainModelMin->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMin->item(i, 0)->setText(
                    deviceLocale->toString(
                        data[i].toDouble(), 'f', 3));
        mainModelMin->item(i, 1)->setText(data[i].toString());
        if (i >= 1) {
            if (data[i] == data[i - 1]) {
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
                    "ORDER BY round(" + operation + measurementCapitalized + ", 6) "
                    + order + ", year, month, day LIMIT " + QString::number(limit));
    }

    return _dbHandler->getResultsFromDatabase(
                "SELECT (max" + measurementCapitalized + " - min" + measurementCapitalized + ") "
                "FROM " + databaseName + " " + monthCondition + " "
                "ORDER BY round(max" + measurementCapitalized + " - min" + measurementCapitalized + ", 6) "
                + order + ", year, month, day LIMIT " + QString::number(limit));

}

std::vector<QVariant> DataExplorator::getValues(QString query, int limit) {
    return _dbHandler->getResultsFromDatabase(query, limit);
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
                    "ORDER BY round(" + operation + measurementCapitalized + ", 6) "
                     + order + ", year ASC, month ASC, day ASC LIMIT " + QString::number(limit));
    }

    return _dbHandler->getResultsFromDatabase(
                "SELECT date FROM " + databaseName + " " + monthCondition + " "
                "ORDER BY round(max" + measurementCapitalized + " - min" + measurementCapitalized + ", 6) "
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

QString DataExplorator::conditionFromWidgets() {
    QString condition = "";
    QString timePeriodCondition = "";
    if (dewPointRadioButton->isChecked()) {
        condition = "WHERE minDewPoint IS NOT NULL";
    }
    else if (humidexRadioButton->isChecked()) {
        condition = "WHERE minHumidex IS NOT NULL";
    }
    else if (pressureRadioButton->isChecked()) {
        condition = "WHERE minPressure > 900";
    }

    if (monthComboBox->currentIndex() < 12) {
        timePeriodCondition = "month = " + QString::number(monthComboBox->currentIndex() + 1);
    }
    else if (monthComboBox->currentText() == "Printemps") {
        timePeriodCondition = "month BETWEEN 3 AND 5";
    }
    else if (monthComboBox->currentText() == "Été") {
        timePeriodCondition = "month BETWEEN 6 AND 8";
    }
    else if (monthComboBox->currentText() == "Automne") {
        timePeriodCondition = "month BETWEEN 9 AND 11";
    }
    else if (monthComboBox->currentText() == "Hiver") {
        timePeriodCondition = "month IN (1, 2, 12)";
    }

    if (condition == "" && timePeriodCondition == "") {
        return "";
    }
    else if (condition == "") {
        return "WHERE " + timePeriodCondition;
    }
    else if (timePeriodCondition == "") {
        return condition;
    }
    else {
        return condition + " AND " + timePeriodCondition;
    }
}

void DataExplorator::displayHeadersFromRadioButtons() {
    QString measurement = "de la température";
    QString measurementPlusOperation = "de la température maximale";
    QString article = "";

    if (temperatureRadioButton->isChecked()) {
        measurement = "température";
        article = "de la ";
    }
    else if (humidityRadioButton->isChecked()) {
        measurement = "humidité";
        article = "de l'";
    }
    else if (dewPointRadioButton->isChecked()) {
        measurement = "point de rosée";
        article = "du ";
    }
    else if (humidexRadioButton->isChecked()) {
        measurement = "humidex";
        article = "de l'";
    }
    else {
        measurement = "pression";
        article = "de la ";
    }

    if (maximumRadioButton->isChecked()) {
        measurementPlusOperation = measurement + " max.";
        mainModelMax->horizontalHeaderItem(0)->setText("Max. " + article + measurementPlusOperation);
        mainModelMin->horizontalHeaderItem(0)->setText("Min. " + article + measurementPlusOperation);
    }
    else if (minimumRadioButton->isChecked()) {
        measurementPlusOperation = measurement + " min.";
        mainModelMax->horizontalHeaderItem(0)->setText("Max. " + article + measurementPlusOperation);
        mainModelMin->horizontalHeaderItem(0)->setText("Min. " + article + measurementPlusOperation);
    }
    else if (averageRadioButton->isChecked()) {
        measurementPlusOperation = measurement + " moy.";
        mainModelMax->horizontalHeaderItem(0)->setText("Max. " + article + measurementPlusOperation);
        mainModelMin->horizontalHeaderItem(0)->setText("Min. " + article + measurementPlusOperation);
    }
    else {
        mainModelMax->horizontalHeaderItem(0)->setText("Var. max. " + article + measurement);
        mainModelMin->horizontalHeaderItem(0)->setText("Var. min. " + article + measurement);
    }
}

void DataExplorator::displayMoreResults() {
    const int maximumNumberOfRecords = maxNumberOfRecords(interiorCheckBox->isChecked());
    int increment = numberOfResults < 20 ? 5 : numberOfResults < 50 ? 10 : numberOfResults < 150 ? 25 : 50;
    if (numberOfResults < maximumNumberOfRecords) {
        numberOfResults += increment;
        mainModelMax->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        mainModelMin->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        if (lastOperationWasFromCustomQuery) fillBoards(customQueryLineEdit->text());
        else fillBoards();
    }
}

void DataExplorator::displayLessResults() {
    const int maximumNumberOfRecords = maxNumberOfRecords(interiorCheckBox->isChecked());
    int decrement = numberOfResults > 150 ? 50 : numberOfResults > 50 ? 25 : numberOfResults > 20 ? 10 : 5;
    if (numberOfResults > decrement) {
        numberOfResults -= decrement;
        mainModelMax->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        mainModelMin->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        if (lastOperationWasFromCustomQuery) fillBoards(customQueryLineEdit->text());
        else fillBoards();
    }
}

void DataExplorator::changeDisplayMonth() {
    // Function executed only when `monthComboBox` changes value
    const int maximumNumberOfRecords = maxNumberOfRecords(interiorCheckBox->isChecked());
    mainModelMax->setRowCount(std::min(maximumNumberOfRecords, mainModelMax->rowCount()));
    mainModelMin->setRowCount(std::min(maximumNumberOfRecords, mainModelMin->rowCount()));
    fillBoards();
}

int DataExplorator::maxNumberOfRecords(bool indoor) {
    QString operation = operationFromRadioButtons();
    QString measurementCapitalized = measurementCapitalizedFromRadioButtons();
    QString condition = conditionFromWidgets();
    indoor = indoor || measurementCapitalized == "Pressure";
    QString tableName = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";

    if (operation == "diff") {
        QString extendedCondition = "min" + measurementCapitalized + " IS NOT NULL "
                                    "AND max" + measurementCapitalized + " IS NOT NULL ";
        if (condition == "") {
            condition = "WHERE " + extendedCondition;
        }
        else {
            condition += " AND " + extendedCondition;
        }
        return _dbHandler->getResultFromDatabase(
                    "SELECT COUNT(*) FROM ("
                    "SELECT min" + measurementCapitalized + ", max" + measurementCapitalized + " "
                    "FROM " + tableName + " " + condition + ")").toInt();
    }
    else {
        return _dbHandler->getResultFromDatabase("SELECT COUNT(" + operation + measurementCapitalized + ") "
                                                 "FROM " + tableName + " " + condition).toInt();
    }
}

void DataExplorator::sendRequest() {
    lastOperationWasFromCustomQuery = true;
    fillBoards(customQueryLineEdit->text());
}

void DataExplorator::selectQueryParams() {
    lastOperationWasFromCustomQuery = false;
    fillBoards();
}

void DataExplorator::selectCustomQuery() {
    lastOperationWasFromCustomQuery = true;
    fillBoards(customQueryLineEdit->text());
}

