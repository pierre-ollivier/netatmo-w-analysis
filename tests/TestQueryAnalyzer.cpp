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
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) ASC");

    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) ASC"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) ASC");

    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) DESC"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) ASC");
}
void TestQueryAnalyzer::testToDESC() {

}
