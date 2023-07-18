#include "QueryAnalyzer.h"

QueryAnalyzer::QueryAnalyzer()
{

}

QString QueryAnalyzer::dateQueryFromMeasurementQuery(QString measurementQuery) {
    QString measurementQueryLowered = measurementQuery.toLower();
    int indexOfSelect = measurementQueryLowered.indexOf("select");
    int indexOfFrom = measurementQueryLowered.indexOf("from");
    if (indexOfSelect >= 0 && indexOfFrom >= 0) {
        return "SELECT date FROM" + measurementQuery.right(measurementQuery.size() - indexOfFrom - 4);
    }
    return "";
}

QString QueryAnalyzer::toASC(QString query) {
    QString queryLowered = query.toLower();
    if (queryLowered.contains(QString("desc"))) {
        query = query.replace("desc", "ASC", Qt::CaseInsensitive);
    }
    if (queryLowered.contains("order by")) {
        if (!query.contains("asc", Qt::CaseInsensitive)) {
            query += " ASC";
        }
    }
    else {
        int indexOfSelect = queryLowered.indexOf("select");
        int indexOfFrom = queryLowered.indexOf("from");
        QString measurement = queryLowered.mid(indexOfSelect + 6, indexOfFrom - indexOfSelect - 6);
        query += " ORDER BY " + measurement + " ASC";
    }
    return query;
}

QString QueryAnalyzer::toDESC(QString query) {
    QString queryLowered = query.toLower();
    if (queryLowered.contains(QString("asc"))) {
        query = query.replace("asc", "DESC", Qt::CaseInsensitive);
    }
    if (queryLowered.contains("order by")) {
        if (!query.contains("desc", Qt::CaseInsensitive)) {
            query += " DESC";
        }
    }
    else {
        int indexOfSelect = queryLowered.indexOf("select");
        int indexOfFrom = queryLowered.indexOf("from");
        QString measurement = query.mid(indexOfSelect + 6, indexOfFrom - indexOfSelect - 6);
        query += " ORDER BY " + measurement + " DESC";
    }
    return query;
}

QString QueryAnalyzer::measurementTypeFromQuery(QString query) {
    /*
     * Returns the measurement present in the query.
     * The return is a lowercase QString: "temperature", "humidity", "dewpoint", "humidex", "pressure", "co2" or "noise".
     * If no measurement is found, the return is "".
     */
    QString queryLowered = query.toLower();
    int indexOfSelect = queryLowered.indexOf("select");
    int indexOfFrom = queryLowered.indexOf("from");
    QString measurementPart = queryLowered.mid(indexOfSelect + 7, indexOfFrom - indexOfSelect - 7);
    measurementPart = measurementPart.replace("(", "");
    measurementPart = measurementPart.replace(")", "");

    while (measurementPart.left(3) == "max" || measurementPart.left(3) == "min" || measurementPart.left(3) == "avg") {
        measurementPart = measurementPart.right(measurementPart.length() - 3);
    }

    if (measurementPart.left(11) == "temperature") return "temperature";
    if (measurementPart.left(8) == "humidity") return "humidity";
    if (measurementPart.left(8) == "dewpoint") return "dewpoint";
    if (measurementPart.left(7) == "humidex") return "humidex";
    if (measurementPart.left(8) == "pressure") return "pressure";
    if (measurementPart.left(3) == "co2") return "co2";
    if (measurementPart.left(5) == "noise") return "noise";
    return "";
}
