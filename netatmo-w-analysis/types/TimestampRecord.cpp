#include "TimestampRecord.h"
#include <math.h>
#include <QDateTime>

TimestampRecord::TimestampRecord(
        long long timestamp,
        double temperature,
        int humidity
        )
{
    _timestamp = timestamp;
    _temperature = temperature;
    _humidity = humidity;
    _date = QDateTime::fromSecsSinceEpoch(timestamp).date();
    _time = QDateTime::fromSecsSinceEpoch(timestamp).time();
}

long long TimestampRecord::timestamp() {
    return _timestamp;
}
QDate TimestampRecord::date() {
    return _date;
}
QTime TimestampRecord::time() {
    return _time;
}
int TimestampRecord::year() {
    return _date.year();
}
int TimestampRecord::month() {
    return _date.month();
}
int TimestampRecord::day() {
    return _date.day();
}
int TimestampRecord::decade() {
    int result = day() <= 10 ? 1 : day() <= 20 ? 2 : 3;
    return result + 3 * (month() - 1);
}
int TimestampRecord::weekNumber() {
    return _date.weekNumber();
}
int TimestampRecord::hour() {
    return _time.hour();
}
int TimestampRecord::minute() {
    return _time.minute();
}
int TimestampRecord::second() {
    return _time.second();
}

double TimestampRecord::temperature() {
    return _temperature;
}
int TimestampRecord::humidity() {
    return _humidity;
}
double TimestampRecord::dewPoint() {
    return ((112 + 0.9 * _temperature) * pow(0.01 * _humidity, 0.125) + 0.1 * _temperature - 112);
}
double TimestampRecord::humidex() {
    return (_temperature + 0.5555 * (6.11 * exp(5417.7530 * (1/273.16 - 1/(273.15 + dewPoint()))) - 10));
}

QString TimestampRecord::toString() {
    return ("Date : " + date().toString("dd/MM/yyyy") + "\n"
            + "Heure : " + time().toString("hh:mm:ss") + "\n"
            + "Température : " + QString::number(_temperature) + " °C" + "\n"
            + "Humidité : " + QString::number(_humidity) + " %" + "\n"
            );
}
