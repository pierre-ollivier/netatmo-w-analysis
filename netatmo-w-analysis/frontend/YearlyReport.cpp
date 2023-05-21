#include "YearlyReport.h"
#include <QDate>
#include <QDebug>

YearlyReport::YearlyReport()
{
    mainView = new QTableView();
    mainModel = new QStandardItemModel();

    deviceLocale = new QLocale();

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
        double txx = getMaxMaxTemperatureByDate(date.day(), date.month()).toDouble();
        int txxYear = getMaxMaxTemperatureYearByDate(date.day(), date.month(), txx);
        mainModel->setItem(row, 0, new QStandardItem(deviceLocale->toString(txx, 'f', 1)));
        mainModel->setItem(row, 1, new QStandardItem(QString::number(txxYear)));
        mainModel->setVerticalHeaderItem(row, new QStandardItem(date.toString("dd/MM")));
    }
}

QVariant YearlyReport::getMaxMaxTemperatureByDate(int day, int month) {
    return dbHandler->getResultFromDatabase(
                "SELECT max(maxTemperature) FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " AND month = " + QString::number(month));
}

int YearlyReport::getMaxMaxTemperatureYearByDate(int day, int month, double maxMaxTemperature) {
    return dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(day) + " "
                "AND month = " + QString::number(month) + " "
                "AND maxTemperature = " + QString::number(maxMaxTemperature)).toInt();
}
