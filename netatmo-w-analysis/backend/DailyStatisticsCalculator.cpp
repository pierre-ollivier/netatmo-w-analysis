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

// avg noise

double DailyStatisticsCalculator::getAvgNoiseFromDate(QDate date) {
    return indoorAverageCalculator->getAverageMeasurementFromDate(date, "noise");
}

// indoor measurement

QPair<QVariant, long long> DailyStatisticsCalculator::getMaxIndoorMeasurementInfoFromDate(
    QDate date,
    QList<IntTimestampRecord> records,
    std::function<QVariant(IntTimestampRecord)> measurementGetter,
    bool localTime,
    int utcOffsetH
    ) {
    const long long firstTimestamp = localTime ? getFirstTimestampFromDate(date) :
                                         getFirstTimestampFromDateWithUTCOffset(date, utcOffsetH);
    const long long lastTimestamp = localTime ? getFirstTimestampFromDate(date.addDays(1)) :
                                        getFirstTimestampFromDateWithUTCOffset(date.addDays(1), utcOffsetH);

    QVariant currentMaxMeasurement = QVariant(-DBL_MAX);
    long long currentMaxTimestamp = firstTimestamp;

    for (IntTimestampRecord record : records) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter(record).toDouble() > currentMaxMeasurement.toDouble()) {
            currentMaxMeasurement = measurementGetter(record);
            currentMaxTimestamp = record.timestamp();
        }
    }
    return qMakePair(currentMaxMeasurement, currentMaxTimestamp);
}

QPair<QVariant, long long> DailyStatisticsCalculator::getMinIndoorMeasurementInfoFromDate(
    QDate date,
    QList<IntTimestampRecord> records,
    std::function<QVariant(IntTimestampRecord)> measurementGetter,
    bool localTime,
    int utcOffsetH
    ) {
    const long long firstTimestamp = localTime ? getFirstTimestampFromDate(date) :
                                         getFirstTimestampFromDateWithUTCOffset(date, utcOffsetH);
    const long long lastTimestamp = localTime ? getFirstTimestampFromDate(date.addDays(1)) :
                                        getFirstTimestampFromDateWithUTCOffset(date.addDays(1), utcOffsetH);

    QVariant currentMinMeasurement = QVariant(DBL_MAX);
    long long currentMinTimestamp = firstTimestamp;

    for (IntTimestampRecord record : records) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter(record).toDouble() < currentMinMeasurement.toDouble()) {
            currentMinMeasurement = measurementGetter(record);
            currentMinTimestamp = record.timestamp();
        }
    }
    return qMakePair(currentMinMeasurement, currentMinTimestamp);
}

// outdoor measurement

QPair<QVariant, long long> DailyStatisticsCalculator::getMaxOutdoorMeasurementInfoFromDate(
    QDate date,
    QList<ExtTimestampRecord> records,
    std::function<QVariant(ExtTimestampRecord)> measurementGetter,
    bool localTime,
    int utcOffsetH
    ) {
    const long long firstTimestamp = localTime ? getFirstTimestampFromDate(date) :
                                         getFirstTimestampFromDateWithUTCOffset(date, utcOffsetH);
    const long long lastTimestamp = localTime ? getFirstTimestampFromDate(date.addDays(1)) :
                                        getFirstTimestampFromDateWithUTCOffset(date.addDays(1), utcOffsetH);

    QVariant currentMaxMeasurement = QVariant(-DBL_MAX);
    long long currentMaxTimestamp = firstTimestamp;

    for (ExtTimestampRecord record : records) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter(record).toDouble() > currentMaxMeasurement.toDouble()) {
            currentMaxMeasurement = measurementGetter(record);
            currentMaxTimestamp = record.timestamp();
        }
    }
    return qMakePair(currentMaxMeasurement, currentMaxTimestamp);
}

QPair<QVariant, long long> DailyStatisticsCalculator::getMinOutdoorMeasurementInfoFromDate(
    QDate date,
    QList<ExtTimestampRecord> records,
    std::function<QVariant(ExtTimestampRecord)> measurementGetter,
    bool localTime,
    int utcOffsetH
    ) {
    const long long firstTimestamp = localTime ? getFirstTimestampFromDate(date) :
                                         getFirstTimestampFromDateWithUTCOffset(date, utcOffsetH);
    const long long lastTimestamp = localTime ? getFirstTimestampFromDate(date.addDays(1)) :
                                        getFirstTimestampFromDateWithUTCOffset(date.addDays(1), utcOffsetH);

    QVariant currentMinMeasurement = QVariant(DBL_MAX);
    long long currentMinTimestamp = firstTimestamp;

    for (ExtTimestampRecord record : records) {
        if (record.timestamp() >= firstTimestamp && record.timestamp() <= lastTimestamp
            && measurementGetter(record).toDouble() < currentMinMeasurement.toDouble()) {
            currentMinMeasurement = measurementGetter(record);
            currentMinTimestamp = record.timestamp();
        }
    }
    return qMakePair(currentMinMeasurement, currentMinTimestamp);
}

double DailyStatisticsCalculator::getAvgOutdoorMeasurementFromDate(QDate date, QList<ExtTimestampRecord> records, QString measurement) {
    return outdoorAverageCalculator->getOutdoorAverageMeasurementFromDate(date, records, measurement);
}

double DailyStatisticsCalculator::getAvgIndoorMeasurementFromDate(QDate date, QList<IntTimestampRecord> records, QString measurement) {
    return indoorAverageCalculator->getIndoorAverageMeasurementFromDate(date, records, measurement);
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
