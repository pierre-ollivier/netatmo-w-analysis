#ifndef DAILYAVERAGECALCULATOR_H
#define DAILYAVERAGECALCULATOR_H

#include <QDate>

class DailyAverageCalculator
{
public:
    DailyAverageCalculator(bool indoor);
    double getAverageTemperatureFromDate(QDate date);
    double getAverageHumidityFromDate(QDate date);

    double getFirstTemperatureFromDate(QDate date);
    double getLastTemperatureFromDate(QDate date);
    double getFirstHumidityFromDate(QDate date);
    double getLastHumidityFromDate(QDate date);
    long long getFirstTimestampFromDate(QDate date);
    long long getLastTimestampFromDate(QDate date);

    QString indoorOrOutdoor();

private:
    bool _indoor;
};

#endif // DAILYAVERAGECALCULATOR_H
