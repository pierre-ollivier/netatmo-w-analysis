#include "DailyRecord.h"
#include <QDateTime>

QTime timestampToTime(long long timestampIns) {
    QDateTime dt;
    dt.setSecsSinceEpoch(timestampIns);
    return dt.time();
}

DailyRecord::DailyRecord(
        QDate date,
        double maxTemperature,
        double minTemperature,
        double avgTemperature,
        int maxHumidity,
        int minHumidity,
        double avgHumidity,
        long long maxTemperatureTimestamp,
        long long minTemperatureTimestamp,
        long long maxHumidityTimestamp,
        long long minHumidityTimestamp
        )
{
    _date = date;
    _maxTemperature = maxTemperature;
    _minTemperature = minTemperature;
    _avgTemperature = avgTemperature;
    _maxHumidity = maxHumidity;
    _minHumidity = minHumidity;
    _avgHumidity = avgHumidity;
    _maxTemperatureTimestamp = maxTemperatureTimestamp;
    _minTemperatureTimestamp = minTemperatureTimestamp;
    _maxHumidityTimestamp = maxHumidityTimestamp;
    _minHumidityTimestamp = minHumidityTimestamp;
}

QDate DailyRecord::date() {
    return _date;
}

int DailyRecord::year() {
    return _date.year();
}
int DailyRecord::month() {
    return _date.month();
}
int DailyRecord::day() {
    return _date.day();
}
int DailyRecord::decade() {
    int result = _date.day() <= 10 ? 1 : _date.day() <= 20 ? 2 : 3;
    return result + 3 * (_date.month() - 1);
}
int DailyRecord::weekNumber() {
    return _date.weekNumber();
}
long long DailyRecord::timestamp() {
    QDateTime dt(_date);
    dt.setTime(QTime(12, 0));
    return dt.toMSecsSinceEpoch();
}

double DailyRecord::maxTemperature() {
    return _maxTemperature;
}
double DailyRecord::minTemperature() {
    return _minTemperature;
}
double DailyRecord::avgTemperature() {
    return _avgTemperature;
}

int DailyRecord::maxHumidity() {
    return _maxHumidity;
}
int DailyRecord::minHumidity() {
    return _minHumidity;
}
double DailyRecord::avgHumidity() {
    return _avgHumidity;
}

long long DailyRecord::maxTemperatureTimestamp() {
    return _maxTemperatureTimestamp;
}
long long DailyRecord::minTemperatureTimestamp() {
    return _minTemperatureTimestamp;
}

long long DailyRecord::maxHumidityTimestamp() {
    return _maxHumidityTimestamp;
}
long long DailyRecord::minHumidityTimestamp() {
    return _minHumidityTimestamp;
}


QTime DailyRecord::maxTemperatureTime() {
    return timestampToTime(_maxTemperatureTimestamp);
}
QTime DailyRecord::minTemperatureTime() {
    return timestampToTime(_minTemperatureTimestamp);
}

QTime DailyRecord::maxHumidityTime() {
    return timestampToTime(_maxHumidityTimestamp);
}
QTime DailyRecord::minHumidityTime() {
    return timestampToTime(_minHumidityTimestamp);
};
