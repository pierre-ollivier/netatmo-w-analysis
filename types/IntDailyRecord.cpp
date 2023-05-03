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
        double maxPressure,
        double minPressure,
        double avgPressure,
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
                maxTemperatureTimestamp,
                minTemperatureTimestamp,
                maxHumidityTimestamp,
                minHumidityTimestamp
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
