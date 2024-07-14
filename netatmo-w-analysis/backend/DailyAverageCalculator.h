#ifndef DAILYAVERAGECALCULATOR_H
#define DAILYAVERAGECALCULATOR_H

#include <QDate>
#include <QObject>

#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class DailyAverageCalculator : public QObject
{
public:
    DailyAverageCalculator(QString pathToDatabase, bool indoor);
    DailyAverageCalculator(QObject *parent, QString pathToDatabase, bool indoor);
    double getAverageMeasurementFromDate(QDate date, QString measurementType);

    double getFirstMeasurementFromDate(QDate date, QString measurementType);
    double getLastMeasurementFromDate(QDate date, QString measurementType);
    long long getFirstTimestampFromDate(QDate date);
    long long getLastTimestampFromDate(QDate date);

    QString indoorOrOutdoor();

private:
    DatabaseHandler *dbHandler;
    QString _pathToDatabase;
    bool _indoor;
};

#endif // DAILYAVERAGECALCULATOR_H
