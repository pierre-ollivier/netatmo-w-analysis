#ifndef DAILYAVERAGECALCULATOR_H
#define DAILYAVERAGECALCULATOR_H

#include <QDate>

class DailyAverageCalculator
{
public:
    DailyAverageCalculator(bool indoor);
    double getAverageMeasurementFromDate(QDate date, QString measurementType);

    double getFirstMeasurementFromDate(QDate date, QString measurementType);
    double getLastMeasurementFromDate(QDate date, QString measurementType);
    long long getFirstTimestampFromDate(QDate date);
    long long getLastTimestampFromDate(QDate date);

    QString indoorOrOutdoor();

private:
    bool _indoor;
};

#endif // DAILYAVERAGECALCULATOR_H
