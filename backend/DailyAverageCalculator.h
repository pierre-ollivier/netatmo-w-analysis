#ifndef DAILYAVERAGECALCULATOR_H
#define DAILYAVERAGECALCULATOR_H

#include <QDate>

class DailyAverageCalculator
{
public:
    DailyAverageCalculator(bool indoor);
    double getAverageExtTemperatureFromDate(QDate date);

private:
    bool _indoor;
};

#endif // DAILYAVERAGECALCULATOR_H
