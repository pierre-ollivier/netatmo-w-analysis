#include "CumulativeAggregator.h"

extern QString PATH_TO_COPY_DATABASE;
extern int BASE_BISSEXTILE_YEAR;

const QMap<QString, QString> measurementToPascalCase = {
    {"temperature", "Temperature"},
    {"humidity", "Humidity"},
    {"dewpoint", "DewPoint"},
    {"humidex", "Humidex"},
    {"pressure", "Pressure"},
    {"co2", "CO2"},
    {"noise", "Noise"},
};

QString dateCondition(QDate beginDate, QDate endDate, QString dateColumn = "date") {
    /*
     * Returns a condition to select rows from a SQLite database where the date represented
     * in `dateColumn` (in format day/month/year) is between beginDate and endDate inclusive.
     */

    return (
        " WHERE DATE(substr(" + dateColumn + ",7,4)"
        + "||'-'||substr(" + dateColumn + ",4,2)"
        + "||'-'||substr(" + dateColumn + ",1,2))"
        + " BETWEEN DATE('" + beginDate.toString(Qt::ISODate) + "')"
        + " AND DATE('" + endDate.toString(Qt::ISODate) + "')"
        );
}

QString dateCondition(int beginMonth, int beginDay, int endMonth, int endDay, QString monthColumn = "month", QString dayColumn = "day") {
    /*
     * Returns a condition to select rows from a SQLite database where the months/days represented
     * in `monthColumn` and `dayColumn` is between (beginMonth, beginDay) and (endMonth, endDay) inclusive.
     * End months/days anterior to begin months/days are supported. The result will consider the interval
     * formed by these two months/days and containing January 1st.
     */

    if (beginMonth < endMonth || (beginMonth == endMonth && beginDay <= endDay)) {
        return (
            " WHERE (" + monthColumn + ", " + dayColumn + ")"
            + " BETWEEN (" + QString::number(beginMonth) + ", " + QString::number(beginDay) + ")"
            + " AND (" + QString::number(endMonth) + ", " + QString::number(endDay) + ")"
        );
    }

    else {
        return (
            " WHERE ((" + monthColumn + ", " + dayColumn + ")"
            + " BETWEEN (" + QString::number(beginMonth) + ", " + QString::number(beginDay) + ")"
            + " AND (12, 31)"
            + " OR (" + monthColumn + ", " + dayColumn + ") BETWEEN (1, 1)"
            + " AND (" + QString::number(endMonth) + ", " + QString::number(endDay) + "))"
        );
    }
}

CumulativeAggregator::CumulativeAggregator(QObject *parent) : QObject(parent) {
    dbHandler = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);
}

QString CumulativeAggregator::measurementQuery(QString measurementType, QString measurementOption, int year, bool indoor) {
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

QString CumulativeAggregator::measurementQuery(
    QString measurementType,
    QString measurementOption,
    QDate beginDate,
    QDate endDate,
    bool indoor
) {

    QString measurement = "";

    if (measurementOption == "diff") {
        measurement = "(max" + measurementToPascalCase[measurementType] + " - min" + measurementToPascalCase[measurementType] + ")";
    }
    else {
        measurement = measurementOption + measurementToPascalCase[measurementType];
    }

    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return (
        "SELECT "
        + measurement
        + " FROM "
        + table
        + dateCondition(beginDate, endDate)
        + " ORDER BY year, month, day"
        );
}

QString CumulativeAggregator::measurementQuery(
    QString measurementType,
    QString measurementOption,
    int beginMonth,
    int beginDay,
    int endMonth,
    int endDay,
    bool indoor,
    bool excludeCurrentYear
    ) {

    QString measurement = "";

    if (measurementOption == "diff") {
        measurement = "(max" + measurementToPascalCase[measurementType] + " - min" + measurementToPascalCase[measurementType] + ")";
    }
    else {
        measurement = measurementOption + measurementToPascalCase[measurementType];
    }

    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return (
        "SELECT "
        + measurement
        + " FROM "
        + table
        + dateCondition(beginMonth, beginDay, endMonth, endDay)
        + (excludeCurrentYear? " AND year < " + QDate::currentDate().toString("yyyy") : "")
        + " ORDER BY year, month, day"
        );
}

QString CumulativeAggregator::measurementQuery(QString measurementType, QString measurementOption, bool indoor, bool excludeCurrentYear) {
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
    return "SELECT date FROM " + table + " WHERE year = " + QString::number(year) + " ORDER BY year, month, day";
}

QString CumulativeAggregator::dateQuery(QDate beginDate, QDate endDate, bool indoor) {
    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return (
        "SELECT date FROM "
            + table
            + dateCondition(beginDate, endDate)
            + " ORDER BY year, month, day"
    );
}

QString CumulativeAggregator::dateQuery(bool indoor, bool excludeCurrentYear) {
    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    QString exclusionCondition = "";
    if (excludeCurrentYear) {
        exclusionCondition = " WHERE year < " + QDate::currentDate().toString("yyyy");
    }
    return "SELECT date FROM " + table + exclusionCondition + " ORDER BY year, month, day";
}

QString CumulativeAggregator::dateQuery(
    int beginMonth,
    int beginDay,
    int endMonth,
    int endDay,
    bool indoor,
    bool excludeCurrentYear
) {
    QString table = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    QString exclusionCondition = "";
    if (excludeCurrentYear) {
        exclusionCondition = " AND year < " + QDate::currentDate().toString("yyyy");
    }
    return (
        "SELECT date FROM "
        + table
        + dateCondition(beginMonth, beginDay, endMonth, endDay)
        + exclusionCondition
        + " ORDER BY year, month, day"
        );
}

QMap<QDate, int> CumulativeAggregator::countMeasurementsMeetingCriteria(
    QString measurementType,
    QString measurementOption,
    QDate beginDate,
    QDate endDate,
    std::function<bool(double)> criteria,
    bool indoor,
    bool cumulative
    ) {
    QString _measurementQuery = measurementQuery(measurementType, measurementOption, beginDate, endDate, indoor);
    QString _dateQuery = dateQuery(beginDate, endDate, indoor);

    std::vector<QVariant> measurementResults = dbHandler->getResultsFromDatabase(_measurementQuery);
    std::vector<QVariant> dateResults = dbHandler->getResultsFromDatabase(_dateQuery);

    QMap<QDate, int> counts = QMap<QDate, int>();
    int count = 0;

    for (unsigned int i = 0; i < measurementResults.size(); i++) {
        if (measurementResults[i].isNull()) continue;
        int increment = criteria(measurementResults[i].toDouble()) ? 1 : 0;
        count = cumulative ? count + increment : increment;
        counts[QDate::fromString(dateResults[i].toString(), "dd/MM/yyyy")] = count;
    }

    return counts;
}

QMap<QDate, double> CumulativeAggregator::countMeasurementsMeetingCriteriaAveraged(
    QString measurementType,
    QString measurementOption,
    int beginMonth,
    int beginDay,
    int endMonth,
    int endDay,
    std::function<bool(double)> criteria,
    bool indoor,
    bool excludeCurrentYear,
    bool cumulative
    ) {
    QString _measurementQuery = measurementQuery(
        measurementType,
        measurementOption,
        beginMonth,
        beginDay,
        endMonth,
        endDay,
        indoor,
        excludeCurrentYear
    );
    QString _dateQuery = dateQuery(beginMonth, beginDay, endMonth, endDay, indoor, excludeCurrentYear);

    std::vector<QVariant> measurementResults = dbHandler->getResultsFromDatabase(_measurementQuery);
    std::vector<QVariant> dateResults = dbHandler->getResultsFromDatabase(_dateQuery);

    QMap<QDate, double> counts = QMap<QDate, double>();
    QMap<QDate, int> datesCounts = QMap<QDate, int>();

    QDate minDate = QDate(BASE_BISSEXTILE_YEAR, beginMonth, beginDay);
    QDate maxDate = QDate(BASE_BISSEXTILE_YEAR, endMonth, endDay);
    if (maxDate < minDate) maxDate = maxDate.addYears(1);

    for (QDate date = minDate; date <= maxDate; date = date.addDays(1)) {
        counts[date] = 0.;
        datesCounts[date] = 0;
    }

    for (unsigned int i = 0; i < measurementResults.size(); i++) {
        if (measurementResults[i].isNull()) continue;
        QDate date = QDate::fromString(dateResults[i].toString(), "dd/MM/yyyy");
        date = date.addYears(BASE_BISSEXTILE_YEAR - date.year());
        datesCounts[date]++;
        if (criteria(measurementResults[i].toDouble())) counts[date]++;
    }

    for (QDate date : counts.keys()) {
        counts[date] = datesCounts[date] == 0 ? 0. : counts[date] / datesCounts[date];
    }

    QMap<QDate, double> aggregatedCounts = QMap<QDate, double>();
    double partialCount = 0.;
    for (QDate date = minDate; date <= maxDate; date = date.addDays(1)) {
        QDate dateInReferenceYear = date.addYears(BASE_BISSEXTILE_YEAR - date.year());
        partialCount = cumulative ? partialCount + counts[dateInReferenceYear] : counts[dateInReferenceYear];
        aggregatedCounts[date] = partialCount;
    }

    return aggregatedCounts;
}


QMap<QDate, double> CumulativeAggregator::aggregateMeasurements(
    QString measurementType,
    QString measurementOption,
    QDate startDate,
    QDate endDate,
    bool indoor,
    std::function<double(std::vector<QVariant>)> aggregationFunction,
    bool recoverMissingConstantValues
    ) {
    QString _measurementQuery = measurementQuery(
        measurementType,
        measurementOption,
        startDate,
        endDate,
        indoor
    );
    QString _dateQuery = dateQuery(startDate, endDate, indoor);

    std::vector<QVariant> measurementResults = dbHandler->getResultsFromDatabase(_measurementQuery);
    std::vector<QVariant> dateResults = dbHandler->getResultsFromDatabase(_dateQuery);
    std::vector<QDate> dates = std::vector<QDate>();

    for (QVariant date: dateResults) {
        dates.push_back(
            QDate::fromString(date.toString(), "dd/MM/yyyy")
        );
    }

    QMap<QDate, double> results = QMap<QDate, double>();

    for (unsigned int i = 0; i < measurementResults.size(); i++) {
        std::vector<QVariant> truncatedMeasurements = measurementResults;
        truncatedMeasurements.resize(i + 1);
        results[dates[i]] = aggregationFunction(truncatedMeasurements);
    }

    if (recoverMissingConstantValues) {
        for (unsigned int i = 0; i < measurementResults.size() - 1; i++) {
            if (dates[i].daysTo(dates[i + 1]) == 1 || abs(results[dates[i]] - results[dates[i + 1]]) > 1e-6) {
                continue;
            }
            double constantValue = results[dates[i]];
            for (QDate date = dates[i].addDays(1); date < dates[i + 1]; date = date.addDays(1)) {
                results[date] = constantValue;
            }
        }
    }

    return results;
}

QMap<QDate, double> CumulativeAggregator::aggregateMeasurementsAveraged(
        QMap<int, QMap<QDate, double>> valuesByYear,
        QDate minDate,
        QDate maxDate,
        bool includeCurrentYear
    ) {
    QMap<QDate, double> averageValues = QMap<QDate, double>();

    for (
        QDate date = minDate;
        date <= maxDate;
        date = date.addDays(1)
    ) {
        int numberOfValues = 0;
        double sumOfValues = 0.;

        for (int year : valuesByYear.keys()) {
            if (year == QDate::currentDate().year() && !includeCurrentYear) continue;

            if (valuesByYear[year].contains(date.addYears(year - BASE_BISSEXTILE_YEAR))) {
                numberOfValues++;
                sumOfValues += valuesByYear[year][date.addYears(year - BASE_BISSEXTILE_YEAR)];
            }
        }

        if (numberOfValues > 0) averageValues[date] = sumOfValues / numberOfValues;
    }
    return averageValues;
}
