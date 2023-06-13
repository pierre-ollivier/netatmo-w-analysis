#include "NormalComputer.h"

NormalComputer::NormalComputer(DatabaseHandler *dbHandler)
{
    _dbHandler = dbHandler;
}

double NormalComputer::normalMaximalTemperatureByMovingAverage(QString tableName, QDate date, int daysCount) {
    QString query = "SELECT AVG(maxTemperature) FROM " + tableName + " ";
    int dayGap = daysCount / 2; // number of days on each side
    QDate beginDate = date.addDays(-dayGap), endDate = date.addDays(dayGap);

    // TODO
    if (beginDate.year() == endDate.year()) {
        query += "WHERE 100*month + day BETWEEN "
                + QString::number(100 * beginDate.month() + beginDate.day())
                + " AND "
                + QString::number(100 * endDate.month() + endDate.day());
    }

    return _dbHandler->getResultFromDatabase(query).toDouble();
}
