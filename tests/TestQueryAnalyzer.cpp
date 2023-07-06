#include "TestQueryAnalyzer.h"
#include <QTest>

TestQueryAnalyzer::TestQueryAnalyzer()
{
    analyzer = new QueryAnalyzer();
}

void TestQueryAnalyzer::testDateQueryFromMeasurementQuery() {
    QCOMPARE(analyzer->dateQueryFromMeasurementQuery(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2)"),
             "SELECT date FROM IndoorDailyRecords ORDER BY ((maxTemperature - minTemperature)/2)");

    QCOMPARE(analyzer->dateQueryFromMeasurementQuery(
                 "Select ((maxTemperature - minTemperature)/2) from IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2)"),
             "SELECT date FROM IndoorDailyRecords ORDER BY ((maxTemperature - minTemperature)/2)");
}
void TestQueryAnalyzer::testToASC() {

}
void TestQueryAnalyzer::testToDESC() {

}
