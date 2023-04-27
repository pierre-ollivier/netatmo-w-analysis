#include "DailyStatisticsCalculator.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>

DailyStatisticsCalculator::DailyStatisticsCalculator()
{
    dbHandler = new DatabaseHandler("netatmo_analysis.db");
}

// max temperature

double DailyStatisticsCalculator::getMaxTemperatureFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, 6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(temperature) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxTemperatureTimestampFromDate(QDate date, double maxTemperature, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, 6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE temperature = " + QString::number(maxTemperature)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxTemperatureTimestampFromDate(QDate date, bool indoor) {
    double maxTemperature = getMaxTemperatureFromDate(date, indoor);
    return getMaxTemperatureTimestampFromDate(date, maxTemperature, indoor);
}

// min temperature

double DailyStatisticsCalculator::getMinTemperatureFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, -6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(temperature) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinTemperatureTimestampFromDate(QDate date, double minTemperature, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, -6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE temperature = " + QString::number(minTemperature)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinTemperatureTimestampFromDate(QDate date, bool indoor) {
    double minTemperature = getMinTemperatureFromDate(date, indoor);
    return getMinTemperatureTimestampFromDate(date, minTemperature, indoor);
}

// max humidity

int DailyStatisticsCalculator::getMaxHumidityFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(humidity) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMaxHumidityTimestampFromDate(QDate date, int maxHumidity, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE humidity = " + QString::number(maxHumidity)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxHumidityTimestampFromDate(QDate date, bool indoor) {
    double maxHumidity = getMaxHumidityFromDate(date, indoor);
    return getMaxHumidityTimestampFromDate(date, maxHumidity, indoor);
}

// min humidity

int DailyStatisticsCalculator::getMinHumidityFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(humidity) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMinHumidityTimestampFromDate(QDate date, int minHumidity, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE humidity = " + QString::number(minHumidity)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinHumidityTimestampFromDate(QDate date, bool indoor) {
    double minHumidity = getMinHumidityFromDate(date, indoor);
    return getMinHumidityTimestampFromDate(date, minHumidity, indoor);
}

// max dew point

double DailyStatisticsCalculator::getMaxDewPointFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(round(dewPoint, 3)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxDewPointTimestampFromDate(QDate date, double maxDewPoint, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(dewPoint, 3) = " + QString::number(maxDewPoint)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxDewPointTimestampFromDate(QDate date, bool indoor) {
    double maxDewPoint = getMaxDewPointFromDate(date, indoor);
    return getMaxDewPointTimestampFromDate(date, maxDewPoint, indoor);
}

// min dew point

double DailyStatisticsCalculator::getMinDewPointFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(dewPoint) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinDewPointTimestampFromDate(QDate date, double minDewPoint, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE dewPoint = " + QString::number(minDewPoint)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinDewPointTimestampFromDate(QDate date, bool indoor) {
    double minDewPoint = getMinDewPointFromDate(date, indoor);
    return getMinDewPointTimestampFromDate(date, minDewPoint, indoor);
}

// others

long long DailyStatisticsCalculator::getFirstTimestampFromDate(QDate date) {
    QDateTime dt = QDateTime(date);
    return dt.toSecsSinceEpoch();
}

long long DailyStatisticsCalculator::getFirstTimestampFromDateWithUTCOffset(QDate date, int offsetFromUTCInHours) {
    QDateTime dt = QDateTime(date, QTime(0, 0, 0), QTimeZone(-3600 * offsetFromUTCInHours));
    return dt.toSecsSinceEpoch();
}
