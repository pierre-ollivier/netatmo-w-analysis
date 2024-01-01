#include "NormalComputer.h"
#include <cmath>
#include <QDebug>

NormalComputer::NormalComputer(DatabaseHandler *dbHandler)
{
    _dbHandler = dbHandler;
}

int NormalComputer::minYear(QString tableName) {
    const QString query = "SELECT MIN(year) FROM " + tableName;
    return _dbHandler->getResultFromDatabase(query).toInt();
}

double NormalComputer::normalMeasurementByMovingAverage(
        QString tableName,
        QDate date,
        QString measurement,
        int daysCount) {

    QString query = "SELECT AVG(" + measurement + ") FROM " + tableName + " ";
    int dayGap = daysCount / 2; // number of days on each side
    const QDate beginDate = date.addDays(-dayGap), endDate = date.addDays(dayGap);

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
    const QDate beginDate = date.addDays(-dayGap), endDate = date.addDays(dayGap);

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

double NormalComputer::stdevMeasurementByMovingAverage(
        QString tableName,
        QDate date,
        QString measurement,
        double average,
        int daysCount) {

    QString query = "SELECT AVG(" + measurement + " * " + measurement + ") "
                    "- " + QString::number(pow(average, 2)) + " FROM " + tableName + " ";
    int dayGap = daysCount / 2; // number of days on each side
    const  QDate beginDate = date.addDays(-dayGap), endDate = date.addDays(dayGap);

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

QList<double> NormalComputer::createStandardDeviationList(
        QString tableName,
        QString measurement,
        QList<double> averagesList,
        int daysCount) {
    QList<double> result = QList<double>();
    for (QDate date = QDate(2020, 1, 1); date.year() < 2021; date = date.addDays(1)) {
        result.append(stdevMeasurementByMovingAverage(
                          tableName,
                          date,
                          measurement,
                          averagesList[date.dayOfYear() - 1],
                          daysCount));
    }
    return result;
}

QList<double> NormalComputer::createValuesFromCurrentYear(QString tableName, QString measurement) {
    QList<double> result = QList<double>();
    const QDate date = QDate::currentDate();
    const QDate firstDayOfCurrentYear = date.addDays(1 - date.dayOfYear());
    const QDate lastDate = _dbHandler->getLatestDateTimeFromDatabase(tableName, measurement).date();

    for (QDate d = firstDayOfCurrentYear; d <= lastDate; d = d.addDays(1)) {
        QString query = "SELECT " + measurement + " FROM " + tableName + " ";
        query += "WHERE day = " + QString::number(d.day()) + " ";
        query += "AND month = " + QString::number(d.month()) + " ";
        query += "AND year = " + QString::number(d.year());
        result.append(_dbHandler->getResultFromDatabase(query).toDouble());
    }
    return result;
}

QList<double> NormalComputer::createValuesFromGivenYear(int year, QString tableName, QString measurement) {
    QList<double> result = QList<double>();
    const QDate firstDayOfCurrentYear = QDate(year, 1, 1);
    const QDate lastDayOfCurrentYear = QDate(year, 12, 31);
    QDate firstDate = _dbHandler->getFirstDateTimeFromDatabase(tableName, measurement).date();
    QDate lastDate = _dbHandler->getLatestDateTimeFromDatabase(tableName, measurement).date();

    if (firstDayOfCurrentYear >= firstDate) {
        firstDate = firstDayOfCurrentYear;
    }
    else {
        // since the left part of the graph is missing, we add nans at the beginning to avoid an offset
        for (int _ = 0; _ < firstDayOfCurrentYear.daysTo(firstDate); _++) {
            result.append(nan(""));
        }
    }
    if (lastDayOfCurrentYear < lastDate) lastDate = lastDayOfCurrentYear;

    QString query = "SELECT " + measurement + " FROM " + tableName + " ";
    query += "WHERE 10000 * year + 100 * month + day BETWEEN " + firstDate.toString("yyyyMMdd") + " ";
    query += "AND " + lastDate.toString("yyyyMMdd") + " ";
    query += "ORDER BY year, month, day";
    std::vector<QVariant> dbResults = _dbHandler->getResultsFromDatabase(query);

    QString dateQuery = "SELECT date FROM " + tableName + " ";
    dateQuery += "WHERE 10000 * year + 100 * month + day BETWEEN " + firstDate.toString("yyyyMMdd") + " ";
    dateQuery += "AND " + lastDate.toString("yyyyMMdd") + " ";
    dateQuery += "ORDER BY year, month, day";
    std::vector<QVariant> dates = _dbHandler->getResultsFromDatabase(dateQuery);

    int addedNans = 0;
    for (unsigned int dateIndex = 0; dateIndex < dates.size(); dateIndex++) {
        QDate date = QDate::fromString(dates[dateIndex].toString(), "dd/MM/yyyy");
        while (date > firstDate.addDays(dateIndex + addedNans)) {
            // Since the dates don't match, one date before is missing.
            // Thus, we add a NaN value before to account for the missing data point.
            addedNans++;
            result.append(nan(""));
        }
        result.append(dbResults[dateIndex].toDouble());
    }

    return result;
}
