#include "DailyStatisticsCalculator.h"
#include "float.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>
#include <functional>
#include "../types/ExtTimestampRecord.h"
#include "../types/IntTimestampRecord.h"

DailyStatisticsCalculator::DailyStatisticsCalculator(QString pathToDatabase, DatabaseHandler *dbHandler)
{
    _pathToDatabase = pathToDatabase;
    _dbHandler = dbHandler;
    indoorAverageCalculator = new DailyAverageCalculator(this, pathToDatabase, true);
    outdoorAverageCalculator = new DailyAverageCalculator(this, pathToDatabase, false);
}

DailyStatisticsCalculator::DailyStatisticsCalculator(QObject *parent, QString pathToDatabase, DatabaseHandler *dbHandler) : QObject(parent)
{
    _pathToDatabase = pathToDatabase;
    _dbHandler = dbHandler;
    indoorAverageCalculator = new DailyAverageCalculator(this, pathToDatabase, true);
    outdoorAverageCalculator = new DailyAverageCalculator(this, pathToDatabase, false);
}

// max temperature

double DailyStatisticsCalculator::getMaxTemperatureFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, 6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(temperature) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxTemperatureTimestampFromDate(QDate date, double maxTemperature, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, 6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE temperature = " + QString::number(maxTemperature, 'f', 6)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxTemperatureTimestampFromDate(QDate date, bool indoor) {
    double maxTemperature = getMaxTemperatureFromDate(date, indoor);
    return getMaxTemperatureTimestampFromDate(date, maxTemperature, indoor);
}

QPair<double, long long> DailyStatisticsCalculator::getMaxTemperatureInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> maxTemperatureInfo = getMaxMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.temperature(); },
            [](IntTimestampRecord record){ return record.temperature(); }),
        false,
        6
        );
    return qMakePair(maxTemperatureInfo.first.toDouble(), maxTemperatureInfo.second);
}

// min temperature

double DailyStatisticsCalculator::getMinTemperatureFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, -6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(temperature) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinTemperatureTimestampFromDate(QDate date, double minTemperature, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, -6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE temperature = " + QString::number(minTemperature, 'f', 6)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinTemperatureTimestampFromDate(QDate date, bool indoor) {
    double minTemperature = getMinTemperatureFromDate(date, indoor);
    return getMinTemperatureTimestampFromDate(date, minTemperature, indoor);
}

QPair<double, long long> DailyStatisticsCalculator::getMinTemperatureInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> minTemperatureInfo = getMinMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.temperature(); },
            [](IntTimestampRecord record){ return record.temperature(); }),
        false,
        -6
        );
    return qMakePair(minTemperatureInfo.first.toDouble(), minTemperatureInfo.second);
}

// avg temperature

double DailyStatisticsCalculator::getAvgTemperatureFromDate(QDate date, QList<ExtTimestampRecord> records, bool indoor) {
    if (indoor) return indoorAverageCalculator->getAverageMeasurementFromDate(date, "temperature");
    DailyAverageCalculator calculator = DailyAverageCalculator(this, _pathToDatabase, false);
    return calculator.getOutdoorAverageMeasurementFromDate(date, records, "temperature");
}

// max humidity

int DailyStatisticsCalculator::getMaxHumidityFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(humidity) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMaxHumidityTimestampFromDate(QDate date, int maxHumidity, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE humidity = " + QString::number(maxHumidity)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxHumidityTimestampFromDate(QDate date, bool indoor) {
    double maxHumidity = getMaxHumidityFromDate(date, indoor);
    return getMaxHumidityTimestampFromDate(date, maxHumidity, indoor);
}

QPair<int, long long> DailyStatisticsCalculator::getMaxHumidityInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> maxHumidityInfo = getMaxMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.humidity(); },
            [](IntTimestampRecord record){ return record.humidity(); })
        );
    return qMakePair(maxHumidityInfo.first.toInt(), maxHumidityInfo.second);
}

// min humidity

int DailyStatisticsCalculator::getMinHumidityFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(humidity) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMinHumidityTimestampFromDate(QDate date, int minHumidity, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE humidity = " + QString::number(minHumidity)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinHumidityTimestampFromDate(QDate date, bool indoor) {
    double minHumidity = getMinHumidityFromDate(date, indoor);
    return getMinHumidityTimestampFromDate(date, minHumidity, indoor);
}

QPair<int, long long> DailyStatisticsCalculator::getMinHumidityInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> minHumidityInfo = getMinMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.humidity(); },
            [](IntTimestampRecord record){ return record.humidity(); })
        );
    return qMakePair(minHumidityInfo.first.toInt(), minHumidityInfo.second);
}


// avg humidity

double DailyStatisticsCalculator::getAvgHumidityFromDate(QDate date, bool indoor) {
    if (indoor) return indoorAverageCalculator->getAverageMeasurementFromDate(date, "humidity");
    return outdoorAverageCalculator->getAverageMeasurementFromDate(date, "humidity");
}

// max dew point

double DailyStatisticsCalculator::getMaxDewPointFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(round(dewPoint, 6)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxDewPointTimestampFromDate(QDate date, double maxDewPoint, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(dewPoint, 6) = " + QString::number(maxDewPoint, 'f', 6)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxDewPointTimestampFromDate(QDate date, bool indoor) {
    double maxDewPoint = getMaxDewPointFromDate(date, indoor);
    return getMaxDewPointTimestampFromDate(date, maxDewPoint, indoor);
}

QPair<double, long long> DailyStatisticsCalculator::getMaxDewPointInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> maxDewPointInfo = getMaxMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.dewPoint(); },
            [](IntTimestampRecord record){ return record.dewPoint(); })
        );
    return qMakePair(maxDewPointInfo.first.toDouble(), maxDewPointInfo.second);
}

// min dew point

double DailyStatisticsCalculator::getMinDewPointFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(round(dewPoint, 6)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinDewPointTimestampFromDate(QDate date, double minDewPoint, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(dewPoint, 6) = " + QString::number(minDewPoint, 'f', 6)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinDewPointTimestampFromDate(QDate date, bool indoor) {
    double minDewPoint = getMinDewPointFromDate(date, indoor);
    return getMinDewPointTimestampFromDate(date, minDewPoint, indoor);
}

QPair<double, long long> DailyStatisticsCalculator::getMinDewPointInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> minDewPointInfo = getMinMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.dewPoint(); },
            [](IntTimestampRecord record){ return record.dewPoint(); })
        );
    return qMakePair(minDewPointInfo.first.toDouble(), minDewPointInfo.second);
}

// avg dew point

double DailyStatisticsCalculator::getAvgDewPointFromDate(QDate date, bool indoor) {
    if (indoor) return indoorAverageCalculator->getAverageMeasurementFromDate(date, "dewPoint");
    return outdoorAverageCalculator->getAverageMeasurementFromDate(date, "dewPoint");
}

// max humidex

double DailyStatisticsCalculator::getMaxHumidexFromDate(QDate date, bool indoor) {

    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(round(humidex, 6)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxHumidexTimestampFromDate(QDate date, double maxHumidex, bool indoor) {

    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(humidex, 6) = " + QString::number(maxHumidex, 'f', 6)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxHumidexTimestampFromDate(QDate date, bool indoor) {
    double maxHumidex = getMaxHumidexFromDate(date, indoor);
    return getMaxHumidexTimestampFromDate(date, maxHumidex, indoor);
}

QPair<double, long long> DailyStatisticsCalculator::getMaxHumidexInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> maxHumidexInfo = getMaxMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.humidex(); },
            [](IntTimestampRecord record){ return record.humidex(); })
        );
    return qMakePair(maxHumidexInfo.first.toDouble(), maxHumidexInfo.second);
}

// min humidex

double DailyStatisticsCalculator::getMinHumidexFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(round(humidex, 6)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinHumidexTimestampFromDate(QDate date, double minHumidex, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(humidex, 6) = " + QString::number(minHumidex, 'f', 6)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinHumidexTimestampFromDate(QDate date, bool indoor) {
    double minHumidex = getMinHumidexFromDate(date, indoor);
    return getMinHumidexTimestampFromDate(date, minHumidex, indoor);
}

QPair<double, long long> DailyStatisticsCalculator::getMinHumidexInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> minHumidexInfo = getMinMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return record.humidex(); },
            [](IntTimestampRecord record){ return record.humidex(); })
        );
    return qMakePair(minHumidexInfo.first.toDouble(), minHumidexInfo.second);
}

// avg humidex

double DailyStatisticsCalculator::getAvgHumidexFromDate(QDate date, bool indoor) {
    if (indoor) return indoorAverageCalculator->getAverageMeasurementFromDate(date, "humidex");
    return outdoorAverageCalculator->getAverageMeasurementFromDate(date, "humidex");
}

// max pressure

double DailyStatisticsCalculator::getMaxPressureFromDate(QDate date) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(round(pressure, 1)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxPressureTimestampFromDate(QDate date, double maxPressure) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(pressure, 1) = " + QString::number(maxPressure, 'f', 1)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxPressureTimestampFromDate(QDate date) {
    double maxPressure = getMaxPressureFromDate(date);
    return getMaxPressureTimestampFromDate(date, maxPressure);
}

QPair<double, long long> DailyStatisticsCalculator::getMaxPressureInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> maxPressureInfo = getMaxMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return 0; },
            [](IntTimestampRecord record){ return record.pressure(); })
        );
    return qMakePair(maxPressureInfo.first.toDouble(), maxPressureInfo.second);
}

// min pressure

double DailyStatisticsCalculator::getMinPressureFromDate(QDate date) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(round(pressure, 1)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinPressureTimestampFromDate(QDate date, double minPressure) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(pressure, 1) = " + QString::number(minPressure, 'f', 1)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinPressureTimestampFromDate(QDate date) {
    double minPressure = getMinPressureFromDate(date);
    return getMinPressureTimestampFromDate(date, minPressure);
}

QPair<double, long long> DailyStatisticsCalculator::getMinPressureInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> minPressureInfo = getMinMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return 0; },
            [](IntTimestampRecord record){ return record.pressure(); })
        );
    return qMakePair(minPressureInfo.first.toDouble(), minPressureInfo.second);
}

// avg pressure

double DailyStatisticsCalculator::getAvgPressureFromDate(QDate date) {
    return indoorAverageCalculator->getAverageMeasurementFromDate(date, "pressure");
}

// max CO2

int DailyStatisticsCalculator::getMaxCO2FromDate(QDate date) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(co2) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMaxCO2TimestampFromDate(QDate date, int maxCO2) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE co2 = " + QString::number(maxCO2)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxCO2TimestampFromDate(QDate date) {
    int maxCO2 = getMaxCO2FromDate(date);
    return getMaxCO2TimestampFromDate(date, maxCO2);
}

QPair<int, long long> DailyStatisticsCalculator::getMaxCO2InfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> maxCO2Info = getMaxMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return 0; },
            [](IntTimestampRecord record){ return record.co2(); })
        );
    return qMakePair(maxCO2Info.first.toInt(), maxCO2Info.second);
}

// min CO2

int DailyStatisticsCalculator::getMinCO2FromDate(QDate date) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(co2) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE co2 > 0";
    query += " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMinCO2TimestampFromDate(QDate date, int minCO2) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE co2 = " + QString::number(minCO2)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinCO2TimestampFromDate(QDate date) {
    int minCO2 = getMinCO2FromDate(date);
    return getMinCO2TimestampFromDate(date, minCO2);
}

QPair<int, long long> DailyStatisticsCalculator::getMinCO2InfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> minCO2Info = getMinMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return 0; },
            [](IntTimestampRecord record){ return record.co2(); })
        );
    return qMakePair(minCO2Info.first.toInt(), minCO2Info.second);
}

// avg CO2

double DailyStatisticsCalculator::getAvgCO2FromDate(QDate date) {
    return indoorAverageCalculator->getAverageMeasurementFromDate(date, "co2");
}

// max noise

int DailyStatisticsCalculator::getMaxNoiseFromDate(QDate date) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(noise) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMaxNoiseTimestampFromDate(QDate date, int maxNoise) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE noise = " + QString::number(maxNoise)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMaxNoiseTimestampFromDate(QDate date) {
    int maxNoise = getMaxNoiseFromDate(date);
    return getMaxNoiseTimestampFromDate(date, maxNoise);

}

QPair<int, long long> DailyStatisticsCalculator::getMaxNoiseInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> maxNoiseInfo = getMaxMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return 0; },
            [](IntTimestampRecord record){ return record.noise(); })
        );
    return qMakePair(maxNoiseInfo.first.toInt(), maxNoiseInfo.second);
}

// min noise

int DailyStatisticsCalculator::getMinNoiseFromDate(QDate date) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(noise) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE noise > 0";
    query += " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMinNoiseTimestampFromDate(QDate date, int minNoise) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE noise = " + QString::number(minNoise)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return _dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinNoiseTimestampFromDate(QDate date) {
    int minNoise = getMinNoiseFromDate(date);
    return getMinNoiseTimestampFromDate(date, minNoise);
}

QPair<int, long long> DailyStatisticsCalculator::getMinNoiseInfoFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records
    ) {
    QPair<QVariant, long long> minNoiseInfo = getMinMeasurementInfoFromDate(
        date,
        records,
        qMakePair(
            [](ExtTimestampRecord record){ return 0; },
            [](IntTimestampRecord record){ return record.noise(); })
        );
    return qMakePair(minNoiseInfo.first.toInt(), minNoiseInfo.second);
}

// avg noise

double DailyStatisticsCalculator::getAvgNoiseFromDate(QDate date) {
    return indoorAverageCalculator->getAverageMeasurementFromDate(date, "noise");
}

// measurement

QPair<QVariant, long long> DailyStatisticsCalculator::getMaxMeasurementInfoFromDate(
    QDate date,
    QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records,
    QPair<std::function<QVariant(ExtTimestampRecord)>, std::function<QVariant(IntTimestampRecord)>> measurementGetter,
    bool localTime,
    int utcOffsetH
    ) {
    const long long firstTimestamp = localTime ? getFirstTimestampFromDate(date) :
                                         getFirstTimestampFromDateWithUTCOffset(date, utcOffsetH);
    const long long lastTimestamp = localTime ? getFirstTimestampFromDate(date.addDays(1)) :
                                        getFirstTimestampFromDateWithUTCOffset(date.addDays(1), utcOffsetH);

    QVariant currentMaxMeasurement = QVariant(-DBL_MAX);
    long long currentMaxTimestamp = firstTimestamp;

    for (ExtTimestampRecord record : records.first) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter.first(record).toDouble() > currentMaxMeasurement.toDouble()) {
            currentMaxMeasurement = measurementGetter.first(record);
            currentMaxTimestamp = record.timestamp();
        }
    }
    for (IntTimestampRecord record : records.second) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter.second(record).toDouble() > currentMaxMeasurement.toDouble()) {
            currentMaxMeasurement = measurementGetter.second(record);
            currentMaxTimestamp = record.timestamp();
        }
    }
    return qMakePair(currentMaxMeasurement, currentMaxTimestamp);
}

QPair<QVariant, long long> DailyStatisticsCalculator::getMinMeasurementInfoFromDate(
    QDate date,
    QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records,
    QPair<std::function<QVariant(ExtTimestampRecord)>, std::function<QVariant(IntTimestampRecord)>> measurementGetter,
    bool localTime,
    int utcOffsetH
    ) {
    const long long firstTimestamp = localTime ? getFirstTimestampFromDate(date) :
                                         getFirstTimestampFromDateWithUTCOffset(date, utcOffsetH);
    const long long lastTimestamp = localTime ? getFirstTimestampFromDate(date.addDays(1)) :
                                        getFirstTimestampFromDateWithUTCOffset(date.addDays(1), utcOffsetH);

    QVariant currentMinMeasurement = QVariant(DBL_MAX);
    long long currentMinTimestamp = firstTimestamp;

    for (ExtTimestampRecord record : records.first) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter.first(record).toDouble() < currentMinMeasurement.toDouble()) {
            currentMinMeasurement = measurementGetter.first(record);
            currentMinTimestamp = record.timestamp();
        }
    }
    for (IntTimestampRecord record : records.second) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter.second(record).toDouble() < currentMinMeasurement.toDouble()) {
            currentMinMeasurement = measurementGetter.second(record);
            currentMinTimestamp = record.timestamp();
        }
    }
    return qMakePair(currentMinMeasurement, currentMinTimestamp);
}

double DailyStatisticsCalculator::getAvgMeasurementFromDate(
    QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records, QString measurement, bool indoor
    ) {
    if (indoor) return indoorAverageCalculator->getIndoorAverageMeasurementFromDate(date, records.second, measurement);
    return outdoorAverageCalculator->getOutdoorAverageMeasurementFromDate(date, records.first, measurement);
}

// others
long long DailyStatisticsCalculator::getFirstTimestampFromDate(QDate date) {
    QDateTime dt = QDateTime(date, QTime(0, 0));
    return dt.toSecsSinceEpoch();
}

/**
 * @brief DailyStatisticsCalculator::getFirstTimestampFromDateWithUTCOffset
 * This function returns the timestamp that corresponds to offsetFromUTCInHours hours after midnight UTC on day date.
 * @param date
 * The date as a QDate object
 * @param offsetFromUTCInHours
 * The number of hours after midnight, as an integer. Negative offsets are supported.
 * @return
 * Returns a long long corresponding to the desired timestamp.
 */
long long DailyStatisticsCalculator::getFirstTimestampFromDateWithUTCOffset(QDate date, int offsetFromUTCInHours) {
    QDateTime dt = QDateTime(date, QTime(0, 0, 0), QTimeZone(-3600 * offsetFromUTCInHours));
    return dt.toSecsSinceEpoch();
}
