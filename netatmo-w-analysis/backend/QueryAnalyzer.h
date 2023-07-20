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
    QString measurementTypeFromQuery(QString query);
    QString operationFromQuery(QString query);
    QString horizontalLabelFromQuery(QString query, bool isMax);
    QString horizontalLabelFromQuery(QString measurementType, QString operation, bool isMax);
};

#endif // QUERYANALYZER_H
