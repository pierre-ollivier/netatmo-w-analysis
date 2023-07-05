#include "QueryAnalyzer.h"

QueryAnalyzer::QueryAnalyzer()
{

}

QString QueryAnalyzer::dateQueryFromMeasurementQuery(QString measurementQuery) {
    measurementQuery = measurementQuery.toLower();
    int indexOfSelect = measurementQuery.indexOf("select");
    int indexOfFrom = measurementQuery.indexOf("from");
    if (indexOfSelect >= 0 && indexOfFrom >= 0) {
        return "SELECT date FROM" + measurementQuery.right(measurementQuery.size() - indexOfFrom - 4);
    }
    return "";
}
