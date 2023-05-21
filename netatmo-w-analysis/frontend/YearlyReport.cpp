#include "YearlyReport.h"
#include <QDate>
#include <QDebug>

YearlyReport::YearlyReport()
{
    mainView = new QTableView();
    mainModel = new QStandardItemModel();

    mainView->setModel(mainModel);

    dbHandler = new DatabaseHandler("netatmo-w-analysis/netatmo_analysis.db");

    mainLayout = new QHBoxLayout();
    optionsLayout = new QVBoxLayout();

    mainLayout->addWidget(mainView);
    mainLayout->addLayout(optionsLayout);

    setLayout(mainLayout);

    fillBoard();
}

void YearlyReport::fillBoard() {
    for (QDate date = QDate(2000, 1, 1); date <= QDate(2000, 12, 31); date = date.addDays(1)) {
        int row = date.dayOfYear();
        qDebug() << getMaxTemperatureByDate(date.day(), date.month());
        mainModel->setItem(row, 0, new QStandardItem(getMaxTemperatureByDate(date.day(), date.month()).toString()));
        mainModel->setVerticalHeaderItem(row, new QStandardItem(date.toString("dd/MM")));
    }
}

QVariant YearlyReport::getMaxTemperatureByDate(int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT maxTemperature, year FROM OutdoorDailyRecords "
                "WHERE maxTemperature = ("
                "SELECT max(maxTemperature) FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month) + ")");
}
