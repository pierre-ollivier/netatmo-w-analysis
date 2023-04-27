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

    long long getFirstTimestampFromDate(QDate date);
    long long getFirstTimestampFromDateWithUTCOffset(QDate date, int offsetFromUTCInHours);

private:
    DatabaseHandler *dbHandler;
};

#endif // DAILYSTATISTICSCALCULATOR_H
