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
                   long long maxTemperatureTimestamp,
                   long long minTemperatureTimestamp,
                   long long maxHumidityTimestamp,
                   long long minHumidityTimestamp
                   );

};

#endif // EXTDAILYRECORD_H
