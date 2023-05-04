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
            double maxDewPoint,
            double minDewPoint,
            double avgDewPoint,
            double maxHumidex,
            double minHumidex,
            double avgHumidex,
            double maxPressure,
            double minPressure,
            double avgPressure,
            long long maxTemperatureTimestamp,
            long long minTemperatureTimestamp,
            long long maxHumidityTimestamp,
            long long minHumidityTimestamp,
            long long maxDewPointTimestamp,
            long long minDewPointTimestamp,
            long long maxHumidexTimestamp,
            long long minHumidexTimestamp,
            long long maxPressureTimestamp,
            long long minPressureTimestamp
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
