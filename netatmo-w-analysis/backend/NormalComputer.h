#ifndef NORMALCOMPUTER_H
#define NORMALCOMPUTER_H

#include <QDate>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class NormalComputer
{
public:
    NormalComputer(DatabaseHandler *dbHandler);
    double normalMaximalTemperatureByMovingAverage(QString tableName, QDate date, int daysCount);

private:
    DatabaseHandler *_dbHandler;
};

#endif // NORMALCOMPUTER_H
