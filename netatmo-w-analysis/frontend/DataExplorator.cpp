#include "DataExplorator.h"

DataExplorator::DataExplorator(DatabaseHandler *dbHandler) : QWidget()
{
    _dbHandler = dbHandler;

    mainModel = new QStandardItemModel(5, 2);
    mainModel->setHorizontalHeaderLabels(
                QStringList({"Température maximale", "Date"}));

    mainView = new QTableView();
    mainView->setModel(mainModel);

    layout = new QGridLayout();
    layout->addWidget(mainView, 1, 1);
    setLayout(layout);

    fillBoard();
}

void DataExplorator::fillBoard() {
    std::vector<QVariant> maxTemperatures = _dbHandler->getResultsFromDatabase(
                "SELECT maxTemperature FROM OutdoorDailyRecords WHERE month = 6 "
                "ORDER BY maxTemperature DESC, year, day LIMIT 5");
    std::vector<QVariant> maxTemperaturesDates = _dbHandler->getResultsFromDatabase(
                "SELECT date FROM OutdoorDailyRecords WHERE month = 6 "
                "ORDER BY maxTemperature DESC, year, day LIMIT 5");

    for (int i = 0; i < 5; i++) {
        mainModel->setItem(i, 0, new QStandardItem());
        mainModel->setItem(i, 1, new QStandardItem());
        mainModel->item(i, 0)->setText(maxTemperatures[i].toString());
        mainModel->item(i, 1)->setText(maxTemperaturesDates[i].toString());
    }
}
