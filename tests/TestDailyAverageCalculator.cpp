#include "TestDailyAverageCalculator.h"
#include <QTest>

TestDailyAverageCalculator::TestDailyAverageCalculator()
{
    indoorCalculator = new DailyAverageCalculator("testdb.db", true);
    outdoorCalculator = new DailyAverageCalculator("testdb.db", false);
}

void TestDailyAverageCalculator::testGetFirstMeasurementFromDate() {
    QDate testDate = QDate(2019, 10, 30);
    QCOMPARE(indoorCalculator->getFirstMeasurementFromDate(testDate, "temperature"), 20);
    QCOMPARE(indoorCalculator->getFirstMeasurementFromDate(testDate, "humidity"), 52);
    QCOMPARE(indoorCalculator->getFirstMeasurementFromDate(testDate, "dewPoint"), 9.79640082460483);
    QCOMPARE(indoorCalculator->getFirstMeasurementFromDate(testDate, "humidex"), 21.1848324098339);
    QCOMPARE(indoorCalculator->getFirstMeasurementFromDate(testDate, "pressure"), 1023.4);
    QCOMPARE(indoorCalculator->getFirstMeasurementFromDate(testDate, "co2"), 911);
    QCOMPARE(indoorCalculator->getFirstMeasurementFromDate(testDate, "noise"), 36);

    QCOMPARE(outdoorCalculator->getFirstMeasurementFromDate(testDate, "temperature"), 9.5);
    QCOMPARE(outdoorCalculator->getFirstMeasurementFromDate(testDate, "humidity"), 86);
    QCOMPARE(outdoorCalculator->getFirstMeasurementFromDate(testDate, "dewPoint"), 7.24857714655445);
    QCOMPARE(outdoorCalculator->getFirstMeasurementFromDate(testDate, "humidex"), 9.60855251462557);
}
void TestDailyAverageCalculator::testGetLastMeasurementFromDate() {

}
void TestDailyAverageCalculator::testGetFirstTimestampFromDate() {

}
void TestDailyAverageCalculator::testGetLastTimestampFromDate() {

}
