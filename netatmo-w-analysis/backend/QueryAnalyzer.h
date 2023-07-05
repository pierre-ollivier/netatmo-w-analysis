#ifndef QUERYANALYZER_H
#define QUERYANALYZER_H

#include <QString>

class QueryAnalyzer
{
public:
    QueryAnalyzer();

    QString dateQueryFromMeasurementQuery(QString measurementQuery);
};

#endif // QUERYANALYZER_H
