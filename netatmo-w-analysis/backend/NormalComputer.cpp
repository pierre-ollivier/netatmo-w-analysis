#include "NormalComputer.h"

NormalComputer::NormalComputer(DatabaseHandler *dbHandler)
{
    _dbHandler = dbHandler;
}

double NormalComputer::normalMaximalTemperatureByMovingAverage(QString tableName, QDate date, int daysCount) {
    QString query = "SELECT AVG(maxTemperature) FROM " + tableName;
    int dayGap = daysCount / 2; // number of days on each side
    QDate beginDate = date.addDays(-dayGap), endDate = date.addDays(dayGap);

    // TODO
}
