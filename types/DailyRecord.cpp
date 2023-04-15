#include "DailyRecord.h"
#include <QDateTime>

QTime timestampToTime(int timestampInms) {
    QDateTime dt;
    dt.setMSecsSinceEpoch(timestampInms);
    return dt.time();
}

DailyRecord::DailyRecord()
{

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
int DailyRecord::timestamp() {
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

int DailyRecord::maxTemperatureTimestamp() {
    return _maxTemperatureTimestamp;
}
int DailyRecord::minTemperatureTimestamp() {
    return _minTemperatureTimestamp;
}

int DailyRecord::maxHumidityTimestamp() {
    return _maxHumidityTimestamp;
}
int DailyRecord::minHumidityTimestamp() {
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
