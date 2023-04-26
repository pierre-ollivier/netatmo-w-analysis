#include "DailyStatisticsCalculator.h"
#include <QDateTime>
#include <QTimeZone>

DailyStatisticsCalculator::DailyStatisticsCalculator()
{
    dbHandler = new DatabaseHandler("netatmo_analysis.db");
}

double DailyStatisticsCalculator::getMaxTemperatureFromDate(QDate date, bool indoor) {
    const QString indoorOrOutdoor = indoor? "Indoor" : "Outdoor";
    const long long firstTimestamp = getFirstTimestampFromDateWithUTCOffset(date, 6);
    const long long lastTimestamp = firstTimestamp + 86400;
    QString query = "SELECT max(temperature) FROM " + indoorOrOutdoor + "TimestampRecords";
    query += " WHERE timestamp BETWEEN " + QString::number(firstTimestamp) + " AND " + QString::number(lastTimestamp);
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long getMaxTemperatureTimestampFromDate(QDate date, double maxTemperature, bool indoor) {
    return 0;  // TODO
}
long long getMaxTemperatureTimestampFromDate(QDate date, bool indoor) {
    return 0;  // TODO
}


long long DailyStatisticsCalculator::getFirstTimestampFromDate(QDate date) {
    QDateTime dt = QDateTime(date);
    return dt.toSecsSinceEpoch();
}

long long DailyStatisticsCalculator::getFirstTimestampFromDateWithUTCOffset(QDate date, int offsetFromUTCInHours) {
    QDateTime dt = QDateTime(date, QTime(0, 0, 0), QTimeZone(-3600 * offsetFromUTCInHours));
    return dt.toSecsSinceEpoch();
}
