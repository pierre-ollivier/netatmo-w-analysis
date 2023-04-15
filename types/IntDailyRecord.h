#ifndef INTDAILYRECORD_H
#define INTDAILYRECORD_H

#include "DailyRecord.h"

class IntDailyRecord : public DailyRecord
{
public:
    IntDailyRecord(
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

    double maxPressure();
    double minPressure();
    double avgPressure();

    long long maxPressureTimestamp();
    long long minPressureTimestamp();

    QTime maxPressureTime();
    QTime minPressureTime();

private:
    double _maxPressure;
    double _minPressure;
    double _avgPressure;

    long long _maxPressureTimestamp;
    long long _minPressureTimestamp;
};

#endif // INTDAILYRECORD_H
