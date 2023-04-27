#ifndef DAILYSTATISTICSCALCULATOR_H
#define DAILYSTATISTICSCALCULATOR_H

#include <QDate>
#include "DatabaseHandler.h"

class DailyStatisticsCalculator
{
public:
    DailyStatisticsCalculator();
    double getMaxTemperatureFromDate(QDate date, bool indoor = false);
    long long getMaxTemperatureTimestampFromDate(QDate date, double maxTemperature, bool indoor = false);
    long long getMaxTemperatureTimestampFromDate(QDate date, bool indoor = false);

    double getMinTemperatureFromDate(QDate date, bool indoor = false);
    long long getMinTemperatureTimestampFromDate(QDate date, double minTemperature, bool indoor = false);
    long long getMinTemperatureTimestampFromDate(QDate date, bool indoor = false);

    int getMaxHumidityFromDate(QDate date, bool indoor = false);
    long long getMaxHumidityTimestampFromDate(QDate date, int maxHumidity, bool indoor = false);
    long long getMaxHumidityTimestampFromDate(QDate date, bool indoor = false);

    int getMinHumidityFromDate(QDate date, bool indoor = false);
    long long getMinHumidityTimestampFromDate(QDate date, int minHumidity, bool indoor = false);
    long long getMinHumidityTimestampFromDate(QDate date, bool indoor = false);

    long long getFirstTimestampFromDate(QDate date);
    long long getFirstTimestampFromDateWithUTCOffset(QDate date, int offsetFromUTCInHours);

private:
    DatabaseHandler *dbHandler;
};

#endif // DAILYSTATISTICSCALCULATOR_H
