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
    query = query.toLower();
    if (query.contains(QString("asc"))) {
        query = query.replace("asc", "desc");
    }
    if (query.contains("order by")) {
        if (!query.contains("desc")) {
            query += " desc";
        }
    }
    else {
        int indexOfSelect = query.indexOf("select");
        int indexOfFrom = query.indexOf("from");
        QString measurement = query.mid(indexOfSelect + 6, indexOfFrom - indexOfSelect - 6);
        query += " order by " + measurement + " desc";
    }
    return query;
}
