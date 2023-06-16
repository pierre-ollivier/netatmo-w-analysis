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

private:
    DatabaseHandler *_dbHandler;
};

#endif // NORMALCOMPUTER_H
