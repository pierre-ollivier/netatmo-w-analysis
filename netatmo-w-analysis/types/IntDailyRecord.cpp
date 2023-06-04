#include "IntDailyRecord.h"
#include "DailyRecord.cpp"
#include <cmath>
#include <QDateTime>

IntDailyRecord::IntDailyRecord(
        QDate date,
        double maxTemperature,
        double minTemperature,
        double avgTemperature,
        int maxHumidity,
        int minHumidity,
        double avgHumidity,
        double maxDewPoint,
        double minDewPoint,
        double avgDewPoint,
        double maxHumidex,
        double minHumidex,
        double avgHumidex,
        double maxPressure,
        double minPressure,
        double avgPressure,
        int maxCO2,
        int minCO2,
        double avgCO2,
        int maxNoise,
        int minNoise,
        double avgNoise,
        long long maxTemperatureTimestamp,
        long long minTemperatureTimestamp,
        long long maxHumidityTimestamp,
        long long minHumidityTimestamp,
        long long maxDewPointTimestamp,
        long long minDewPointTimestamp,
        long long maxHumidexTimestamp,
        long long minHumidexTimestamp,
        long long maxPressureTimestamp,
        long long minPressureTimestamp
        ) : DailyRecord(
                date,
                maxTemperature,
                minTemperature,
                avgTemperature,
                maxHumidity,
                minHumidity,
                avgHumidity,
                maxDewPoint,
                minDewPoint,
                avgDewPoint,
                maxHumidex,
                minHumidex,
                avgHumidex,
                maxTemperatureTimestamp,
                minTemperatureTimestamp,
                maxHumidityTimestamp,
                minHumidityTimestamp,
                maxDewPointTimestamp,
                minDewPointTimestamp,
                maxHumidexTimestamp,
                minHumidexTimestamp
                )
{
    _maxPressure = maxPressure;
    _minPressure = minPressure;
    _avgPressure = avgPressure;
    _maxCO2 = maxCO2;
    _minCO2 = minCO2;
    _avgCO2 = avgCO2;
    _maxNoise = maxNoise;
    _minNoise = minNoise;
    _avgNoise = avgNoise;

    _maxPressureTimestamp = maxPressureTimestamp;
    _minPressureTimestamp = minPressureTimestamp;
}

IntDailyRecord::IntDailyRecord(
        QDate date,
        double maxTemperature,
        double minTemperature,
        double avgTemperature,
        int maxHumidity,
        int minHumidity,
        double avgHumidity,
        double maxPressure,
        double minPressure,
        double avgPressure,
        int maxCO2,
        int minCO2,
        double avgCO2,
        int maxNoise,
        int minNoise,
        double avgNoise,
        long long maxTemperatureTimestamp,
        long long minTemperatureTimestamp,
        long long maxHumidityTimestamp,
        long long minHumidityTimestamp,
        long long maxPressureTimestamp,
        long long minPressureTimestamp
        ) : DailyRecord(
                date,
                maxTemperature,
                minTemperature,
                avgTemperature,
                maxHumidity,
                minHumidity,
                avgHumidity,
                nan(""),
                nan(""),
                nan(""),
                nan(""),
                nan(""),
                nan(""),
                maxTemperatureTimestamp,
                minTemperatureTimestamp,
                maxHumidityTimestamp,
                minHumidityTimestamp,
                (long long) nanf(""),
                (long long) nanf(""),
                (long long) nanf(""),
                (long long) nanf("")
                )
{
    _maxPressure = maxPressure;
    _minPressure = minPressure;
    _avgPressure = avgPressure;
    _maxCO2 = maxCO2;
    _minCO2 = minCO2;
    _avgCO2 = avgCO2;
    _maxNoise = maxNoise;
    _minNoise = minNoise;
    _avgNoise = avgNoise;

    _maxPressureTimestamp = maxPressureTimestamp;
    _minPressureTimestamp = minPressureTimestamp;
}

double IntDailyRecord::maxPressure() {
    return _maxPressure;
}
double IntDailyRecord::minPressure() {
    return _minPressure;
}
double IntDailyRecord::avgPressure() {
    return _avgPressure;
}

int IntDailyRecord::maxCO2() {
    return _maxCO2;
}
int IntDailyRecord::minCO2() {
    return _minCO2;
}
double IntDailyRecord::avgCO2() {
    return _avgCO2;
}

int IntDailyRecord::maxNoise() {
    return _maxNoise;
}
int IntDailyRecord::minNoise() {
    return _minNoise;
}
double IntDailyRecord::avgNoise() {
    return _avgNoise;
}

long long IntDailyRecord::maxPressureTimestamp() {
    return _maxPressureTimestamp;
}
long long IntDailyRecord::minPressureTimestamp() {
    return _minPressureTimestamp;
}

QTime IntDailyRecord::maxPressureTime() {
    return timestampToTime(_maxPressureTimestamp);
}
QTime IntDailyRecord::minPressureTime() {
    return timestampToTime(_minPressureTimestamp);
}
