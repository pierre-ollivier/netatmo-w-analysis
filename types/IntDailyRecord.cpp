#include "IntDailyRecord.h"
#include "DailyRecord.cpp"
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
