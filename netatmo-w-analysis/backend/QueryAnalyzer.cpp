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
    query = query.toLower();
    if (query.contains(QString("desc"))) {
        query = query.replace("desc", "asc");
    }
    if (query.contains("order by")) {
        if (!query.contains("asc")) {
            query += " asc";
        }
    }
    else {
        int indexOfSelect = query.indexOf("select");
        int indexOfFrom = query.indexOf("from");
        QString measurement = query.mid(indexOfSelect + 6, indexOfFrom - indexOfSelect - 6);
        query += " order by " + measurement + " asc";
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
