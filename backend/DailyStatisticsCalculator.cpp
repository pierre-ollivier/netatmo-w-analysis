#include "DailyStatisticsCalculator.h"
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>

DailyStatisticsCalculator::DailyStatisticsCalculator(QString pathToDatabase)
{
    _pathToDatabase = pathToDatabase;
    dbHandler = new DatabaseHandler(pathToDatabase);
    indoorAverageCalculator = new DailyAverageCalculator(pathToDatabase, true);
    outdoorAverageCalculator = new DailyAverageCalculator(pathToDatabase, false);
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

// avg temperature

double DailyStatisticsCalculator::getAvgTemperatureFromDate(QDate date, bool indoor) {
    if (indoor) return indoorAverageCalculator->getAverageMeasurementFromDate(date, "temperature");
    return outdoorAverageCalculator->getAverageMeasurementFromDate(date, "temperature");
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
    QString query = "SELECT min(round(dewPoint, 3)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinDewPointTimestampFromDate(QDate date, double minDewPoint, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(dewPoint, 3) = " + QString::number(minDewPoint)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    QString query = "SELECT max(round(humidex, 3)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxHumidexTimestampFromDate(QDate date, double maxHumidex, bool indoor) {

    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(humidex, 3) = " + QString::number(maxHumidex)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    QString query = "SELECT min(round(humidex, 3)) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinHumidexTimestampFromDate(QDate date, double minHumidex, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(humidex, 3) = " + QString::number(minHumidex)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMaxPressureTimestampFromDate(QDate date, double maxPressure) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(pressure, 1) = " + QString::number(maxPressure)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyStatisticsCalculator::getMinPressureTimestampFromDate(QDate date, double minPressure) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE round(pressure, 1) = " + QString::number(minPressure)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    return dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMaxCO2TimestampFromDate(QDate date, int maxCO2) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE co2 = " + QString::number(maxCO2)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMinCO2TimestampFromDate(QDate date, int minCO2) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE co2 = " + QString::number(minCO2)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    return dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMaxNoiseTimestampFromDate(QDate date, int maxNoise) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE noise = " + QString::number(maxNoise)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
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
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toInt();
}

long long DailyStatisticsCalculator::getMinNoiseTimestampFromDate(QDate date, int minNoise) {
    const QString indoorOrOutdoor = "Indoor";
    const long long firstTimestamp = getFirstTimestampFromDate(date);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT min(timestamp) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE noise = " + QString::number(minNoise)
           + " AND timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyStatisticsCalculator::getMinNoiseTimestampFromDate(QDate date) {
    int minNoise = getMinNoiseFromDate(date);
    return getMinNoiseTimestampFromDate(date, minNoise);
}

// avg noise

double DailyStatisticsCalculator::getAvgNoiseFromDate(QDate date) {
    return indoorAverageCalculator->getAverageMeasurementFromDate(date, "noise");
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
