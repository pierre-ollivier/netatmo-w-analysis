#ifndef DAILYRECORD_H
#define DAILYRECORD_H

#include <QDate>
#include <QTime>

class DailyRecord
{
public:
    DailyRecord(
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
            );

    QDate date();

    int year();
    int month();
    int day();
    long long timestamp();

    double maxTemperature();
    double minTemperature();
    double avgTemperature();

    int maxHumidity();
    int minHumidity();
    double avgHumidity();

    long long maxTemperatureTimestamp();
    long long minTemperatureTimestamp();

    long long maxHumidityTimestamp();
    long long minHumidityTimestamp();

    QTime maxTemperatureTime();
    QTime minTemperatureTime();

    QTime maxHumidityTime();
    QTime minHumidityTime();

private:
    QDate _date;

    double _maxTemperature;
    double _minTemperature;
    double _avgTemperature;

    int _maxHumidity;
    int _minHumidity;
    double _avgHumidity;

    long long _maxTemperatureTimestamp;
    long long _minTemperatureTimestamp;

    long long _maxHumidityTimestamp;
    long long _minHumidityTimestamp;



};

#endif // DAILYRECORD_H
