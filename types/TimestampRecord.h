#ifndef TIMESTAMPRECORD_H
#define TIMESTAMPRECORD_H

#include <QDate>

class TimestampRecord
{
public:
    TimestampRecord(
            long long timestamp,
            double temperature,
            int humidity
            );

    long long timestamp();
    QDate date();
    QTime time();
    int year();
    int month();
    int day();
    int hour();
    int minute();
    int second();
    int decade();
    int weekNumber();

    double temperature();
    int humidity();
    double dewPoint();
    double humidex();

private:
    long long _timestamp;
    double _temperature;
    int _humidity;
};

#endif // TIMESTAMPRECORD_H
