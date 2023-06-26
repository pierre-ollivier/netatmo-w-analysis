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
    connect(monthComboBox, SIGNAL(currentIndexChanged(int)), SLOT(fillBoard()));

    mainModel = new QStandardItemModel(5, 2);
    mainModel->setHorizontalHeaderLabels(
                QStringList({"Température maximale", "Date"}));

    mainView = new QTableView();
    mainView->setModel(mainModel);

    layout = new QGridLayout();
    layout->addWidget(mainView, 1, 1, 1, 2);
    layout->addWidget(new QLabel("Mois : "), 2, 1);
    layout->addWidget(monthComboBox, 2, 2);
    setLayout(layout);

    setMinimumSize(400, 400);

    fillBoard();
}

void DataExplorator::fillBoard() {
    QString monthCondition = "";
    QString measurementCapitalized = "Temperature";
    QString operation = "max";
    if (monthComboBox->currentIndex() > 0) {
        monthCondition = "WHERE month = " + QString::number(monthComboBox->currentIndex());
    }
    std::vector<QVariant> maxTemperatures = getValues(operation, measurementCapitalized, monthCondition);
    std::vector<QVariant> maxTemperaturesDates = getValuesDates(operation, measurementCapitalized, monthCondition);

    QString unitWithLeadingSpace = " °C";

    for (int i = 0; i < 5; i++) {
        mainModel->setItem(i, 0, new QStandardItem());
        mainModel->setItem(i, 1, new QStandardItem());
        mainModel->setVerticalHeaderItem(i, new QStandardItem(QString::number(i + 1)));
        mainModel->item(i, 0)->setText(deviceLocale->toString(maxTemperatures[i].toDouble(), 'f', 1) + unitWithLeadingSpace);
        mainModel->item(i, 1)->setText(maxTemperaturesDates[i].toString());
        if (i >= 1) {
            if (maxTemperatures[i] == maxTemperatures[i - 1]) {
                mainModel->verticalHeaderItem(i)->setText(mainModel->verticalHeaderItem(i - 1)->text());
            }
        }
    }
    mainView->resizeColumnsToContents();
}

std::vector<QVariant> DataExplorator::getValues(
        QString operation, QString measurementCapitalized, QString monthCondition) {

    return _dbHandler->getResultsFromDatabase(
                "SELECT " + operation + measurementCapitalized + " FROM OutdoorDailyRecords " + monthCondition + " "
                "ORDER BY " + operation + measurementCapitalized + " DESC, year, day LIMIT 5");
}

std::vector<QVariant> DataExplorator::getValuesDates(
        QString operation, QString measurementCapitalized, QString monthCondition) {

    return _dbHandler->getResultsFromDatabase(
                "SELECT date FROM OutdoorDailyRecords " + monthCondition + " "
                "ORDER BY " + operation + measurementCapitalized + " DESC, year, day LIMIT 5");
}
