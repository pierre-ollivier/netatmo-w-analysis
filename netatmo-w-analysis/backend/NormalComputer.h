#ifndef NORMALCOMPUTER_H
#define NORMALCOMPUTER_H

#include <QDate>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class NormalComputer
{
public:
    NormalComputer(DatabaseHandler *dbHandler);
    double normalMeasurementByMovingAverage(
            QString tableName,
            QDate date,
            QString measurement,
            int daysCount);

    double stdevMeasurementByMovingAverage(
            QString tableName,
            QDate date,
            QString measurement,
            int daysCount);
    QList<double> createAveragesList(QString tableName,
                                     QString measurement,
                                     int daysCount);

    QList<double> createStandardDeviationList(QString tableName,
                                              QString measurement,
                                              int daysCount);

    QList<double> createValuesFromCurrentYear(QString tableName,
                                              QString measurement);

private:
    DatabaseHandler *_dbHandler;
};

#endif // NORMALCOMPUTER_H
