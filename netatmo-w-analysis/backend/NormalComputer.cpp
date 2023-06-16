#include "NormalComputer.h"
#include <cmath>

NormalComputer::NormalComputer(DatabaseHandler *dbHandler)
{
    _dbHandler = dbHandler;
}

double NormalComputer::normalMeasurementByMovingAverage(
        QString tableName,
        QDate date,
        QString measurement,
        int daysCount) {

    QString query = "SELECT AVG(" + measurement + ") FROM " + tableName + " ";
    int dayGap = daysCount / 2; // number of days on each side
    QDate beginDate = date.addDays(-dayGap), endDate = date.addDays(dayGap);

    if (beginDate.year() == endDate.year()) {
        query += "WHERE 100 * month + day BETWEEN "
                + QString::number(100 * beginDate.month() + beginDate.day())
                + " AND "
                + QString::number(100 * endDate.month() + endDate.day());
    }

    else {
        query += "WHERE 100 * month + day BETWEEN "
                + QString::number(100 * beginDate.month() + beginDate.day())
                + " AND 1231 OR 100 * month + day BETWEEN 101 AND "
                + QString::number(100 * endDate.month() + endDate.day());
    }

    return _dbHandler->getResultFromDatabase(query).toDouble();
}

double NormalComputer::stdevMeasurementByMovingAverage(
        QString tableName,
        QDate date,
        QString measurement,
        int daysCount) {

    QString query = "SELECT AVG(" + measurement + " * " + measurement + ") "
                    "- AVG(" + measurement + ") * AVG(" + measurement + ") FROM " + tableName + " ";
    int dayGap = daysCount / 2; // number of days on each side
    QDate beginDate = date.addDays(-dayGap), endDate = date.addDays(dayGap);

    if (beginDate.year() == endDate.year()) {
        query += "WHERE 100 * month + day BETWEEN "
                + QString::number(100 * beginDate.month() + beginDate.day())
                + " AND "
                + QString::number(100 * endDate.month() + endDate.day());
    }

    else {
        query += "WHERE 100 * month + day BETWEEN "
                + QString::number(100 * beginDate.month() + beginDate.day())
                + " AND 1231 OR 100 * month + day BETWEEN 101 AND "
                + QString::number(100 * endDate.month() + endDate.day());
    }

    return std::sqrt(_dbHandler->getResultFromDatabase(query).toDouble());
}
