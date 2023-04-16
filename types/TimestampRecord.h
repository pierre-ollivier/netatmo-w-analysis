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
