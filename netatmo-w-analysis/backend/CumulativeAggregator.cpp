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

QString CumulativeAggregator::measurementQuery(QString measurementType, QString measurementOption, bool indoor, bool excludeCurrentYear) {
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
    QString exclusionCondition = "";
    if (excludeCurrentYear) {
        exclusionCondition = " WHERE year < " + QDate::currentDate().toString("yyyy");
    }
    return "SELECT " + measurement + " FROM " + table + exclusionCondition + " ORDER BY year, month, day";
}

QString CumulativeAggregator::dateQuery(int year, bool indoor) {
    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return "SELECT date FROM " + table + " WHERE year = " + QString::number(year) + " ORDER BY month, day";
}

QString CumulativeAggregator::dateQuery(bool indoor, bool excludeCurrentYear) {
    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    QString exclusionCondition = "";
    if (excludeCurrentYear) {
        exclusionCondition = " WHERE year < " + QDate::currentDate().toString("yyyy");
    }
    return "SELECT date FROM " + table + exclusionCondition + " ORDER BY year, month, day";
}

QMap<QDate, int> CumulativeAggregator::countMeasurementsMeetingCriteria(
    QString measurementType,
    QString measurementOption,
    int year,
    std::function<bool(double)> criteria,
    bool indoor
    ) {
    QString _measurementQuery = measurementQuery(measurementType, measurementOption, year, indoor);
    QString _dateQuery = dateQuery(year, indoor);

    std::vector<QVariant> measurementResults = dbHandler->getResultsFromDatabase(_measurementQuery);
    std::vector<QVariant> dateResults = dbHandler->getResultsFromDatabase(_dateQuery);

    QMap<QDate, int> counts = QMap<QDate, int>();
    int count = 0;

    for (unsigned int i = 0; i < measurementResults.size(); i++) {
        if (measurementResults[i].isNull()) continue;
        if (criteria(measurementResults[i].toDouble())) count++;
        counts[QDate::fromString(dateResults[i].toString(), "dd/MM/yyyy")] = count;
    }

    return counts;
}

QMap<QDate, double> CumulativeAggregator::countMeasurementsMeetingCriteriaAveraged(
    QString measurementType,
    QString measurementOption,
    std::function<bool(double)> criteria,
    bool indoor,
    bool excludeCurrentYear
    ) {
    QString _measurementQuery = measurementQuery(measurementType, measurementOption, indoor, excludeCurrentYear);
    QString _dateQuery = dateQuery(indoor, excludeCurrentYear);

    std::vector<QVariant> measurementResults = dbHandler->getResultsFromDatabase(_measurementQuery);
    std::vector<QVariant> dateResults = dbHandler->getResultsFromDatabase(_dateQuery);

    QMap<QDate, double> counts = QMap<QDate, double>();
    QMap<QDate, int> datesCounts = QMap<QDate, int>();

    for (QDate date = QDate(2024, 1, 1); date < QDate(2025, 1, 1); date = date.addDays(1)) {
        counts[date] = 0.;
        datesCounts[date] = 0;
    }

    for (unsigned int i = 0; i < measurementResults.size(); i++) {
        if (measurementResults[i].isNull()) continue;
        QDate date = QDate::fromString(dateResults[i].toString(), "dd/MM/yyyy");
        date = date.addYears(2024 - date.year());
        datesCounts[date]++;
        if (criteria(measurementResults[i].toDouble())) counts[date]++;
    }

    for (QDate date : counts.keys()) {
        counts[date] = datesCounts[date] == 0 ? 0. : counts[date] / datesCounts[date];
    }

    QMap<QDate, double> aggregatedCounts = QMap<QDate, double>();
    double partialCount = 0.;
    for (QDate date = QDate(2024, 1, 1); date < QDate(2025, 1, 1); date = date.addDays(1)) {
        partialCount += counts[date];
        aggregatedCounts[date] = partialCount;
    }

    return aggregatedCounts;
}
