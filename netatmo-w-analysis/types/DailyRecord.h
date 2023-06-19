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
            );

    QDate date();

    int year();
    int month();
    int day();
    int decade();
    int weekNumber();
    long long timestamp();

    double maxTemperature();
    double minTemperature();
    double avgTemperature();

    int maxHumidity();
    int minHumidity();
    double avgHumidity();

    double maxDewPoint();
    double minDewPoint();
    double avgDewPoint();

    double maxHumidex();
    double minHumidex();
    double avgHumidex();

    long long maxTemperatureTimestamp();
    long long minTemperatureTimestamp();

    long long maxHumidityTimestamp();
    long long minHumidityTimestamp();

    long long maxDewPointTimestamp();
    long long minDewPointTimestamp();

    long long maxHumidexTimestamp();
    long long minHumidexTimestamp();

    QTime maxTemperatureTime();
    QTime minTemperatureTime();

    QTime maxHumidityTime();
    QTime minHumidityTime();

    QTime maxDewPointTime();
    QTime minDewPointTime();

    QTime maxHumidexTime();
    QTime minHumidexTime();

    void setMaxTemperature(double maxTemperature);
    void setMinTemperature(double minTemperature);

    void setMaxTemperatureTimestamp(long long maxTemperatureTimestamp);
    void setMinTemperatureTimestamp(long long minTemperatureTimestamp);

private:
    QDate _date;

    double _maxTemperature;
    double _minTemperature;
    double _avgTemperature;

    int _maxHumidity;
    int _minHumidity;
    double _avgHumidity;

    double _maxDewPoint;
    double _minDewPoint;
    double _avgDewPoint;

    double _maxHumidex;
    double _minHumidex;
    double _avgHumidex;

    long long _maxTemperatureTimestamp;
    long long _minTemperatureTimestamp;

    long long _maxHumidityTimestamp;
    long long _minHumidityTimestamp;

    long long _maxDewPointTimestamp;
    long long _minDewPointTimestamp;

    long long _maxHumidexTimestamp;
    long long _minHumidexTimestamp;

};

#endif // DAILYRECORD_H
