#include "CumulativeAggregator.h"

extern QString PATH_TO_COPY_DATABASE;

CumulativeAggregator::CumulativeAggregator(QObject *parent) : QObject(parent) {
    dbHandler = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);
}

QMap<QDate, int> CumulativeAggregator::countMaxTemperaturesHigherOrEqualThanThreshold(int year, double threshold) {
    QString temperatureQuery = "SELECT maxTemperature FROM OutdoorDailyRecords WHERE year = ";
    temperatureQuery += QString::number(year) + " " + "ORDER BY month, day";
    QString dateQuery = "SELECT date FROM OutdoorDailyRecords WHERE year = ";
    dateQuery += QString::number(year) + " " + "ORDER BY month, day";

    std::vector<QVariant> temperatureResults = dbHandler->getResultsFromDatabase(temperatureQuery);
    std::vector<QVariant> dateResults = dbHandler->getResultsFromDatabase(dateQuery);

    QMap<QDate, int> counts = QMap<QDate, int>();
    int count = 0;

    for (unsigned int i = 0; i < temperatureResults.size(); i++) {
        if (temperatureResults[i].toDouble() >= threshold) count++;
        counts[QDate::fromString(dateResults[i].toString(), "dd/MM/yyyy")] = count;
    }

    return counts;
}

QMap<QDate, int> CumulativeAggregator::countMinTemperaturesHigherOrEqualThanThreshold(int year, double threshold) {
    // TODO
    return countMaxTemperaturesHigherOrEqualThanThreshold(year, threshold);
}

QMap<QDate, int> CumulativeAggregator::countAvgTemperaturesHigherOrEqualThanThreshold(int year, double threshold) {
    // TODO
    return countMaxTemperaturesHigherOrEqualThanThreshold(year, threshold);
}
