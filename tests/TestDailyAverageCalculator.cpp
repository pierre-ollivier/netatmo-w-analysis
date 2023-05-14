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
    QDate testDate = QDate(2019, 10, 31);
    QCOMPARE(indoorCalculator->getLastMeasurementFromDate(testDate, "temperature"), 20.7);
    QCOMPARE(indoorCalculator->getLastMeasurementFromDate(testDate, "humidity"), 56);
    QCOMPARE(indoorCalculator->getLastMeasurementFromDate(testDate, "dewPoint"), 11.5672449958022);
    QCOMPARE(indoorCalculator->getLastMeasurementFromDate(testDate, "humidex"), 22.7372408785317);
    QCOMPARE(indoorCalculator->getLastMeasurementFromDate(testDate, "pressure"), 1016.6);
    QCOMPARE(indoorCalculator->getLastMeasurementFromDate(testDate, "co2"), 965);
    QCOMPARE(indoorCalculator->getLastMeasurementFromDate(testDate, "noise"), 37);

    QCOMPARE(outdoorCalculator->getLastMeasurementFromDate(testDate, "temperature"), 11.5);
    QCOMPARE(outdoorCalculator->getLastMeasurementFromDate(testDate, "humidity"), 90);
    QCOMPARE(outdoorCalculator->getLastMeasurementFromDate(testDate, "dewPoint"), 9.89920702513028);
    QCOMPARE(outdoorCalculator->getLastMeasurementFromDate(testDate, "humidex"), 12.7318686435551);
}
void TestDailyAverageCalculator::testGetFirstTimestampFromDate() {

}
void TestDailyAverageCalculator::testGetLastTimestampFromDate() {

}
