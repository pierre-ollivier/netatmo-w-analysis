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
}

long long TimestampRecord::timestamp() {
    return _timestamp;
}
QDate TimestampRecord::date() {
    QDateTime dt;
    dt.setSecsSinceEpoch(_timestamp);
    return dt.date();
}
QTime TimestampRecord::time() {
    QDateTime dt;
    dt.setSecsSinceEpoch(_timestamp);
    return dt.time();
}
// TODO: maybe store date and time to not recalculate them every time
int TimestampRecord::year() {
    return date().year();
}
int TimestampRecord::month() {
    return date().month();
}
int TimestampRecord::day() {
    return date().day();
}
int TimestampRecord::decade() {
    int result =day() <= 10 ? 1 : day() <= 20 ? 2 : 3;
    return result + 3 * (month() - 1);
}
int TimestampRecord::weekNumber() {
    return date().weekNumber();
}
int TimestampRecord::hour() {
    return time().hour();
}
int TimestampRecord::minute() {
    return time().minute();
}
int TimestampRecord::second() {
    return time().second();
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
