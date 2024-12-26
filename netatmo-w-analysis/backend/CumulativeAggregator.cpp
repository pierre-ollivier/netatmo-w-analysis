#include "CumulativeAggregator.h"

extern QString PATH_TO_COPY_DATABASE;

CumulativeAggregator::CumulativeAggregator(QObject *parent) : QObject(parent) {
    dbHandler = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);
}

QString CumulativeAggregator::measurementQuery(QString measurementType, QString measurementOption, int year, bool indoor) {
    const QMap<QString, QString> measurementToPascalCase = {
        {"temperature", "Temperature"},
        {"humidity", "Humidity"},
        {"dewpoint", "DewPoint"},
        {"humidex", "Humidex"},
        {"pressure", "Pressure"},
        {"co2", "CO2"},
        {"noise", "Noise"},
    };
    QString measurement = "";

    if (measurementOption == "diff") {
        measurement = "(max" + measurementToPascalCase[measurementType] + " - min" + measurementToPascalCase[measurementType] + ")";
    }
    else {
        measurement = measurementOption + measurementToPascalCase[measurementType];
    }

    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return "SELECT " + measurement + " FROM " + table + " WHERE year = " + QString::number(year) + " ORDER BY month, day";
}

QString CumulativeAggregator::dateQuery(int year, bool indoor) {
    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return "SELECT date FROM " + table + " WHERE year = " + QString::number(year) + " ORDER BY month, day";
}

QMap<QDate, int> CumulativeAggregator::countMeasurementsMeetingCriteria(
    QString measurementType,
    QString measurementOption,
    int year,
    std::function<bool(double)> criteria
    ) {
    QString _measurementQuery = measurementQuery(measurementType, measurementOption, year);
    QString _dateQuery = dateQuery(year);

    std::vector<QVariant> measurementResults = dbHandler->getResultsFromDatabase(_measurementQuery);
    std::vector<QVariant> dateResults = dbHandler->getResultsFromDatabase(_dateQuery);

    QMap<QDate, int> counts = QMap<QDate, int>();
    int count = 0;

    for (unsigned int i = 0; i < measurementResults.size(); i++) {
        if (criteria(measurementResults[i].toDouble())) count++;
        counts[QDate::fromString(dateResults[i].toString(), "dd/MM/yyyy")] = count;
    }

    return counts;
}
