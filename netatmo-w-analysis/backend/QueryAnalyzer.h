#ifndef QUERYANALYZER_H
#define QUERYANALYZER_H

#include <QObject>
#include <QString>

class QueryAnalyzer : public QObject
{
public:
    QueryAnalyzer(QObject *parent = nullptr);

    QString dateQueryFromMeasurementQuery(QString measurementQuery);
    QString toASC(QString query);
    QString toDESC(QString query);
    QString measurementTypeFromQuery(QString query);
    QString operationFromQuery(QString query);
    QString horizontalLabelFromQuery(QString query, bool isMax);
    QString horizontalLabelFromQuery(QString measurementType, QString operation, bool isMax);
};

#endif // QUERYANALYZER_H
