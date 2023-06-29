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
                   QStringList({"Max. de la température max.", "Date"}));

    mainModelMin = new QStandardItemModel(5, 2);
    mainModelMin->setHorizontalHeaderLabels(
                   QStringList({"Min. de la température max.", "Date"}));

    mainViewMax = new QTableView();
    mainViewMax->setModel(mainModelMax);
    mainViewMax->setFixedWidth(370);
    mainViewMax->setColumnWidth(0, 220);

    mainViewMin = new QTableView();
    mainViewMin->setModel(mainModelMin);
    mainViewMin->setFixedWidth(370);
    mainViewMin->setColumnWidth(0, 220);

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

    if (dewPointRadioButton->isChecked()) {
        monthCondition = "WHERE minDewPoint IS NOT NULL";
    }
    else if (humidexRadioButton->isChecked()) {
        monthCondition = "WHERE minHumidex IS NOT NULL";
    }
    else if (pressureRadioButton->isChecked()) {
        monthCondition = "WHERE minPressure > 900";
    }

    if (monthComboBox->currentIndex() > 0 && monthCondition != "") {
        monthCondition += " AND month = " + QString::number(monthComboBox->currentIndex());
    }
    else if (monthComboBox->currentIndex() > 0) {
        monthCondition = "WHERE month = " + QString::number(monthComboBox->currentIndex());
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
    displayHeadersFromRadioButtons();

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

        // set colors

        if (humidityRadioButton->isChecked()) {
            mainModelMax->item(i, 0)->setBackground(QBrush(humidityColor(maxMeasurements[i].toDouble())));
            mainModelMin->item(i, 0)->setBackground(QBrush(humidityColor(minMeasurements[i].toDouble())));
        }
        else if (pressureRadioButton->isChecked()) {
            mainModelMax->item(i, 0)->setBackground(QBrush(pressureColor(maxMeasurements[i].toDouble())));
            mainModelMin->item(i, 0)->setBackground(QBrush(pressureColor(minMeasurements[i].toDouble())));
        }
        else {
            mainModelMax->item(i, 0)->setBackground(QBrush(temperatureColor(maxMeasurements[i].toDouble())));
            mainModelMin->item(i, 0)->setBackground(QBrush(temperatureColor(minMeasurements[i].toDouble())));
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
    int increment = numberOfResults < 20 ? 5 : numberOfResults < 50 ? 10 : 25;
    // TODO check that there are still results to display (to compute and recalculate each time)
    numberOfResults += increment;
    mainModelMax->insertRows(mainModelMax->rowCount() - 1, increment);
    mainModelMin->insertRows(mainModelMax->rowCount() - 1, increment);
    fillBoards();
}

void DataExplorator::displayLessResults() {
    int decrement = numberOfResults > 50 ? 25 : numberOfResults > 20 ? 10 : 5;
    if (numberOfResults > decrement) {
        numberOfResults -= decrement;
        mainModelMax->removeRows(mainModelMax->rowCount() - decrement, decrement);
        mainModelMin->removeRows(mainModelMin->rowCount() - decrement, decrement);
        fillBoards();
    }
}

QColor DataExplorator::temperatureColor(double temperature) {

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

QColor DataExplorator::humidityColor(double humidity) {
    return temperatureColor(45 - 0.6 * humidity);
}

QColor DataExplorator::pressureColor(double pressure) {
    return temperatureColor(-15 + (pressure - 960) * 60 / 126);
}
