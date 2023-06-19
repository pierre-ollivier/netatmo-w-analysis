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

QList<double> NormalComputer::createAveragesList(QString tableName, QString measurement, int daysCount) {
    QList<double> result = QList<double>();
    for (QDate date = QDate(2020, 1, 1); date.year() < 2021; date = date.addDays(1)) {
        result.append(normalMeasurementByMovingAverage(
                          tableName,
                          date,
                          measurement,
                          daysCount));
    }
    return result;
}

QList<double> NormalComputer::createStandardDeviationList(QString tableName, QString measurement, int daysCount) {
    QList<double> result = QList<double>();
    for (QDate date = QDate(2020, 1, 1); date.year() < 2021; date = date.addDays(1)) {
        result.append(stdevMeasurementByMovingAverage(
                          tableName,
                          date,
                          measurement,
                          daysCount));
    }
    return result;
}

QList<double> NormalComputer::createValuesFromCurrentYear(QString tableName, QString measurement) {
    QList<double> result = QList<double>();
    QDate date = QDate::currentDate();
    QDate firstDayOfCurrentYear = date.addDays(1 - date.dayOfYear());

    for (QDate d = firstDayOfCurrentYear; d < date; d = d.addDays(1)) {
        QString query = "SELECT " + measurement + " FROM " + tableName + " ";
        query += "WHERE day = " + QString::number(d.day()) + " ";
        query += "AND month = " + QString::number(d.month()) + " ";
        query += "AND year = " + QString::number(d.year());
        result.append(_dbHandler->getResultFromDatabase(query).toDouble());
    }
    return result;
}
