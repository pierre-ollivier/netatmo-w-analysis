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
    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2)"),
             "select ((maxTemperature - minTemperature)/2) from IndoorDailyRecords "
             "order by ((maxTemperature - minTemperature)/2) asc");

    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) ASC"),
             "select ((maxTemperature - minTemperature)/2) from IndoorDailyRecords "
             "order by ((maxTemperature - minTemperature)/2) asc");

    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) DESC"),
             "select ((maxTemperature - minTemperature)/2) from IndoorDailyRecords "
             "order by ((maxTemperature - minTemperature)/2) asc");
}
void TestQueryAnalyzer::testToDESC() {

}
