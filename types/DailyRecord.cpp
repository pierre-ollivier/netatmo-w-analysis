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
        double maxDewPoint,
        double minDewPoint,
        double avgDewPoint,
        double maxHumidex,
        double minHumidex,
        double avgHumidex,
        long long maxTemperatureTimestamp,
        long long minTemperatureTimestamp,
        long long maxHumidityTimestamp,
        long long minHumidityTimestamp,
        long long maxDewPointTimestamp,
        long long minDewPointTimestamp,
        long long maxHumidexTimestamp,
        long long minHumidexTimestamp
        )
{
    _date = date;
    _maxTemperature = maxTemperature;
    _minTemperature = minTemperature;
    _avgTemperature = avgTemperature;
    _maxHumidity = maxHumidity;
    _minHumidity = minHumidity;
    _avgHumidity = avgHumidity;
    _maxDewPoint = maxDewPoint;
    _minDewPoint = minDewPoint;
    _avgDewPoint = avgDewPoint;
    _maxHumidex = maxHumidex;
    _minHumidex = minHumidex;
    _avgHumidex = avgHumidex;
    _maxTemperatureTimestamp = maxTemperatureTimestamp;
    _minTemperatureTimestamp = minTemperatureTimestamp;
    _maxHumidityTimestamp = maxHumidityTimestamp;
    _minHumidityTimestamp = minHumidityTimestamp;
    _maxDewPointTimestamp = maxDewPointTimestamp;
    _minDewPointTimestamp = minDewPointTimestamp;
    _maxHumidexTimestamp = maxHumidexTimestamp;
    _minHumidexTimestamp = minHumidexTimestamp;
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

double DailyRecord::maxDewPoint() {
    return _maxDewPoint;
}
double DailyRecord::minDewPoint() {
    return _minDewPoint;
}
double DailyRecord::avgDewPoint() {
    return _avgDewPoint;
}

double DailyRecord::maxHumidex() {
    return _maxHumidity;
}
double DailyRecord::minHumidex() {
    return _minHumidity;
}
double DailyRecord::avgHumidex() {
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

long long DailyRecord::maxDewPointTimestamp() {
    return _maxDewPointTimestamp;
}
long long DailyRecord::minDewPointTimestamp() {
    return _minDewPointTimestamp;
}

long long DailyRecord::maxHumidexTimestamp() {
    return _maxHumidexTimestamp;
}
long long DailyRecord::minHumidexTimestamp() {
    return _minHumidexTimestamp;
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
}

QTime DailyRecord::maxDewPointTime() {
    return timestampToTime(_maxDewPointTimestamp);
}
QTime DailyRecord::minDewPointTime() {
    return timestampToTime(_minDewPointTimestamp);
}

QTime DailyRecord::maxHumidexTime() {
    return timestampToTime(_maxHumidexTimestamp);
}
QTime DailyRecord::minHumidexTime() {
    return timestampToTime(_minHumidexTimestamp);
}
