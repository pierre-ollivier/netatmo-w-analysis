#ifndef QUERYANALYZER_H
#define QUERYANALYZER_H

#include <QString>

class QueryAnalyzer
{
public:
    QueryAnalyzer();

    QString dateQueryFromMeasurementQuery(QString measurementQuery);
    QString toASC(QString query);
    QString toDESC(QString query);
};

#endif // QUERYANALYZER_H
