#include "ExtDailyRecord.h"

ExtDailyRecord::ExtDailyRecord(
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
        ) : DailyRecord(
                date,
                maxTemperature,
                minTemperature,
                avgTemperature,
                maxHumidity,
                minHumidity,
                avgHumidity,
                maxDewPoint,
                minDewPoint,
                avgDewPoint,
                maxHumidex,
                minHumidex,
                avgHumidex,
                maxTemperatureTimestamp,
                minTemperatureTimestamp,
                maxHumidityTimestamp,
                minHumidityTimestamp,
                maxDewPointTimestamp,
                minDewPointTimestamp,
                maxHumidexTimestamp,
                minHumidexTimestamp
                )
{

}
