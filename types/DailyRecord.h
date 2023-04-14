#ifndef DAILYRECORD_H
#define DAILYRECORD_H

#include <QDate>
#include <QTime>

class DailyRecord
{
public:
    DailyRecord();

    QDate date();

    int year();
    int month();
    int day();

    double maxTemperature();
    double minTemperature();
    double avgTemperature();

    int maxHumidity();
    int minHumidity();
    double avgHumidity();

    int maxTemperatureTimestamp();
    int minTemperatureTimestamp();

    int maxHumidityTimestamp();
    int minHumidityTimestamp();

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

    int _maxTemperatureTimestamp;
    int _minTemperatureTimestamp;

    int _maxHumidityTimestamp;
    int _minHumidityTimestamp;



};

#endif // DAILYRECORD_H
