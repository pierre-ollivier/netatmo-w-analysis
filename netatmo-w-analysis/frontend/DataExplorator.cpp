#include "DataExplorator.h"
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QLabel>
#include "../frontend/CustomItemDelegate.h"
#include "../frontend/ColorUtils.h"
#include "../frontend/QueryBuilder/QueryBuilder.h"

extern const QLocale LOCALE;

DataExplorator::DataExplorator(DatabaseHandler *dbHandler) : QWidget()
{
    _dbHandler = dbHandler;
    analyzer = new QueryAnalyzer(this);

    deviceLocale = new QLocale(LOCALE);

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
    variationRadioButton = new QRadioButton("Variation");

    maximumRadioButton->setChecked(true);

    interiorCheckBox = new QCheckBox("Intérieur");

    customQueryGroupBox = new QGroupBox();
    customQueryLineEdit = new QLineEdit();
    customQueryLayout = new QGridLayout();
    pasteQueryButton = new QPushButton("Coller");
    sendQueryButton = new QPushButton("OK");
    buildQueryButton = new QPushButton("Aide");

    queryParamsSelected = new QRadioButton(this);
    customQuerySelected = new QRadioButton(this);
    queryParamsSelected->setChecked(true);

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
    operationsLayout->addWidget(variationRadioButton, 1, 1);

    operationsGroupBox->setLayout(operationsLayout);

    customQueryLayout = new QGridLayout();
    customQueryLayout->addWidget(new QLabel("Ou bien entrez une requête :"), 0, 0, 1, 20);
    customQueryLayout->addWidget(customQueryLineEdit, 1, 0, 1, 17);
    customQueryLayout->addWidget(pasteQueryButton, 1, 17);
    customQueryLayout->addWidget(sendQueryButton, 1, 18);
    customQueryLayout->addWidget(buildQueryButton, 1, 19);

    customQueryGroupBox->setLayout(customQueryLayout);

    connect(temperatureRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(humidityRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(dewPointRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(humidexRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(pressureRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(maximumRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(minimumRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(averageRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(variationRadioButton, SIGNAL(clicked()), SLOT(selectQueryParams()));
    connect(interiorCheckBox, SIGNAL(clicked()), SLOT(selectQueryParams()));

    connect(sendQueryButton, SIGNAL(clicked()), SLOT(sendRequest()));
    connect(customQueryLineEdit, SIGNAL(returnPressed()), SLOT(sendRequest()));
    connect(buildQueryButton, SIGNAL(clicked()), SLOT(showQueryBuilder()));
    connect(pasteQueryButton, SIGNAL(clicked()), SLOT(pasteQueryFromClipboard()));

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
    if (queryParamsSelected->isChecked()) {
        QString databaseName = databaseFromCheckBox();
        QString operation = operationFromRadioButtons();
        QString measurementCapitalized = measurementCapitalizedFromRadioButtons();
        QString condition = conditionFromWidgets();

        customQueryLineEdit->setText(buildQuery(databaseName, operation, measurementCapitalized, condition));
    }
    fillBoards(customQueryLineEdit->text());
}

void DataExplorator::fillBoards(QString query) {
    QString queryASC = analyzer->toASC(query);
    QString queryDESC = analyzer->toDESC(query);

    const QString unitWithLeadingSpace = unitWithLeadingSpaceFromQuery();

    const int decimalCount = decimalsFromQuery();

    std::vector<QVariant> dataASC = getValues(queryASC, numberOfResults);
    std::vector<QVariant> datesASC = getValuesDates(
                analyzer->dateQueryFromMeasurementQuery(queryASC),
                numberOfResults);
    std::vector<QVariant> dataDESC = getValues(queryDESC, numberOfResults);
    std::vector<QVariant> datesDESC = getValuesDates(
                analyzer->dateQueryFromMeasurementQuery(queryDESC),
                numberOfResults);

    mainModelMax->setRowCount(int(dataDESC.size()));
    mainModelMin->setRowCount(int(dataDESC.size()));

    for (int i = 0; i < int(dataDESC.size()); i++) {
        mainModelMax->setItem(i, 0, new QStandardItem());
        mainModelMax->setItem(i, 1, new QStandardItem());
        mainModelMax->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMax->item(i, 0)->setText(
                    deviceLocale->toString(
                        dataDESC[i].toDouble(), 'f', decimalCount) + unitWithLeadingSpace);
        mainModelMax->item(i, 1)->setText(datesDESC[i].toString());
        if (i >= 1) {
            if (abs(dataDESC[i].toDouble() - dataDESC[i - 1].toDouble()) < 1e-6) {
                mainModelMax->verticalHeaderItem(i)->setText(mainModelMax->verticalHeaderItem(i - 1)->text());
            }
        }

        mainModelMin->setItem(i, 0, new QStandardItem());
        mainModelMin->setItem(i, 1, new QStandardItem());
        mainModelMin->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModelMin->item(i, 0)->setText(
                    deviceLocale->toString(
                        dataASC[i].toDouble(), 'f', decimalCount) + unitWithLeadingSpace);
        mainModelMin->item(i, 1)->setText(datesASC[i].toString());
        if (i >= 1) {
            if (abs(dataASC[i].toDouble() - dataASC[i - 1].toDouble()) < 1e-6) {
                mainModelMin->verticalHeaderItem(i)->setText(mainModelMin->verticalHeaderItem(i - 1)->text());
            }
        }

        // set horizontal labels
        mainModelMax->horizontalHeaderItem(0)->setText(analyzer->horizontalLabelFromQuery(query, true));
        mainModelMin->horizontalHeaderItem(0)->setText(analyzer->horizontalLabelFromQuery(query, false));

        // set colors

        QString measurementType = analyzer->measurementTypeFromQuery(query);

        if (measurementType == "humidity") {
            mainModelMax->item(i, 0)->setBackground(QBrush(ColorUtils::humidityColor(dataDESC[i])));
            mainModelMin->item(i, 0)->setBackground(QBrush(ColorUtils::humidityColor(dataASC[i])));
        }
        else if (measurementType == "pressure") {
            mainModelMax->item(i, 0)->setBackground(QBrush(ColorUtils::pressureColor(dataDESC[i])));
            mainModelMin->item(i, 0)->setBackground(QBrush(ColorUtils::pressureColor(dataASC[i])));
        }
        else if (measurementType == "temperature" || measurementType == "dewpoint" || measurementType == "humidex") {
            mainModelMax->item(i, 0)->setBackground(QBrush(ColorUtils::temperatureColor(dataDESC[i])));
            mainModelMin->item(i, 0)->setBackground(QBrush(ColorUtils::temperatureColor(dataASC[i])));
        }

    }

    mainViewMax->resizeColumnsToContents();
    mainViewMin->resizeColumnsToContents();
}

QString DataExplorator::buildQuery(QString databaseName,
                                   QString operation,
                                   QString measurementCapitalized,
                                   QString monthCondition,
                                   QString order,
                                   int limit) {
    if (operation != "diff") {
        return "SELECT " + operation + measurementCapitalized + " "
               + "FROM " + databaseName + " " + monthCondition + " "
               + "ORDER BY round(" + operation + measurementCapitalized + ", 6) "
               + (order != "" ? order + ", year, month, day" + (limit > 0 ? " LIMIT " + QString::number(limit) : "") : "");
    }

    return "SELECT (max" + measurementCapitalized + " - min" + measurementCapitalized + ") "
           + "FROM " + databaseName + " " + monthCondition + " "
           + "ORDER BY round(max" + measurementCapitalized + " - min" + measurementCapitalized + ", 6) "
           + (order != "" ? order + ", year, month, day" + (limit > 0 ? " LIMIT " + QString::number(limit) : "") : "");
}

QString DataExplorator::buildDateQuery(QString databaseName,
                                       QString operation,
                                       QString measurementCapitalized,
                                       QString monthCondition,
                                       QString order,
                                       int limit) {
    if (operation != "diff") {
        return "SELECT date FROM "
               + databaseName + " " + monthCondition + " "
               + "ORDER BY round(" + operation + measurementCapitalized + ", 6) "
               + (order != "" ? order + ", year, month, day" + (limit > 0 ? " LIMIT " + QString::number(limit) : "") : "");
    }

    return "SELECT date FROM "
           + databaseName + " " + monthCondition + " "
           + "ORDER BY round(max" + measurementCapitalized + " - min" + measurementCapitalized + ", 6) "
           + (order != "" ? order + ", year, month, day" + (limit > 0 ? " LIMIT " + QString::number(limit) : "") : "");
}

std::vector<QVariant> DataExplorator::getValues(QString query, int limit) {
    return _dbHandler->getResultsFromDatabase(query, limit);
}

std::vector<QVariant> DataExplorator::getValuesDates(QString query, int limit) {
    return _dbHandler->getResultsFromDatabase(query, limit);
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
    if (variationRadioButton->isChecked()) return "diff";
    return "";
}

QString DataExplorator::unitWithLeadingSpaceFromQuery() {
    return " " + unitFromMeasurement.value(analyzer->measurementTypeFromQuery(customQueryLineEdit->text()));
}

int DataExplorator::decimalsFromQuery() {
    return decimalsFromMeasurement.value(analyzer->measurementTypeFromQuery(customQueryLineEdit->text()));
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

void DataExplorator::displayMoreResults() {
    const int maximumNumberOfRecords = maxNumberOfRecords();
    int increment = numberOfResults < 20 ? 5 : numberOfResults < 50 ? 10 : numberOfResults < 150 ? 25 : 50;
    if (numberOfResults < maximumNumberOfRecords) {
        numberOfResults += increment;
        mainModelMax->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        mainModelMin->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        fillBoards();
    }
}

void DataExplorator::displayLessResults() {
    const int maximumNumberOfRecords = maxNumberOfRecords();
    int decrement = numberOfResults > 150 ? 50 : numberOfResults > 50 ? 25 : numberOfResults > 20 ? 10 : 5;
    if (numberOfResults > decrement) {
        numberOfResults -= decrement;
        mainModelMax->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        mainModelMin->setRowCount(std::min(maximumNumberOfRecords, numberOfResults));
        fillBoards();
    }
}

void DataExplorator::changeDisplayMonth() {
    // Function executed only when `monthComboBox` changes value
    const int maximumNumberOfRecords = maxNumberOfRecords();
    mainModelMax->setRowCount(std::min(maximumNumberOfRecords, mainModelMax->rowCount()));
    mainModelMin->setRowCount(std::min(maximumNumberOfRecords, mainModelMin->rowCount()));
    fillBoards();
}

int DataExplorator::maxNumberOfRecords() {
    return _dbHandler->getNumberOfResultsFromDatabase(customQueryLineEdit->text());
}

void DataExplorator::sendRequest() {
    customQuerySelected->setChecked(true);
    fillBoards(customQueryLineEdit->text());
}

void DataExplorator::showQueryBuilder() {
    QueryBuilder *builder = new QueryBuilder();
    builder->show();
}

void DataExplorator::selectQueryParams() {
    queryParamsSelected->setChecked(true);
    fillBoards();
}

void DataExplorator::selectCustomQuery() {
    fillBoards(customQueryLineEdit->text());
}

void DataExplorator::pasteQueryFromClipboard() {
    QClipboard *clipboard = QApplication::clipboard();
    customQueryLineEdit->setText(clipboard->text());
}
