#ifndef DAILYAVERAGECALCULATOR_H
#define DAILYAVERAGECALCULATOR_H

#include <QDate>

class DailyAverageCalculator
{
public:
    DailyAverageCalculator(bool indoor);
    double getAverageExtTemperatureFromDate(QDate date);

    double getFirstTemperatureFromDate(QDate date);
    double getLastTemperatureFromDate(QDate date);
    long long getFirstTimestampFromDate(QDate date);
    long long getLastTimestampFromDate(QDate date);

private:
    bool _indoor;
};

#endif // DAILYAVERAGECALCULATOR_H
