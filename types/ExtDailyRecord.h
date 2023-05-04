#ifndef EXTDAILYRECORD_H
#define EXTDAILYRECORD_H

#include "DailyRecord.h"


class ExtDailyRecord : public DailyRecord
{
public:
    ExtDailyRecord(QDate date,
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

};

#endif // EXTDAILYRECORD_H
