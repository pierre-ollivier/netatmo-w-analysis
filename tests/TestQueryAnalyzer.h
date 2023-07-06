#ifndef TESTQUERYANALYZER_H
#define TESTQUERYANALYZER_H

#include <QObject>
#include "../netatmo-w-analysis/backend/QueryAnalyzer.h"

class TestQueryAnalyzer : public QObject
{
    Q_OBJECT

public:
    TestQueryAnalyzer();

private slots:
    void testDateQueryFromMeasurementQuery();
    void testToASC();
    void testToDESC();

private:
    QueryAnalyzer *analyzer;
};

#endif // TESTQUERYANALYZER_H
