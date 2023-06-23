#include "TestNormalComputer.h"
#include <QTest>
#include "../netatmo-w-analysis/backend/NormalComputer.h"

TestNormalComputer::TestNormalComputer()
{

}

void TestNormalComputer::testNormalMeasurementByMovingAverageIndoor() {
    NormalComputer normalComputer(new DatabaseHandler("testdb.db")); // TODO: change this
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "minTemperature",
                 5), (19.7 + 17 + 19.9 + 18.5 + 18.4) / 5);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "maxTemperature",
                 5), (21 + 21.6 + 22.3 + 21.1 + 22.6) / 5);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "minHumidity",
                 5), (58. + 58. + 56. + 59. + 60.) / 5);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "maxHumidity",
                 5), (65. + 67. + 61. + 63. + 68.) / 5);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "minDewPoint",
                 5), 11.3536);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "maxDewPoint",
                 5), 14.1484);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "minHumidex",
                 5), 20.7158);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "maxHumidex",
                 5), 25.145);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 15),
                 "minPressure",
                 5), 1007.28);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 15),
                 "maxPressure",
                 5), 1013.04);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 15),
                 "minCO2",
                 5), 354.4);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 15),
                 "maxCO2",
                 5), 968);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 20),
                 "minNoise",
                 5), 7.2);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 15),
                 "maxNoise",
                 5), 49.4);
}

void TestNormalComputer::testNormalMeasurementByMovingAverageOutdoor() {

}

void TestNormalComputer::testStdevMeasurementByMovingAverageIndoor() {

}

void TestNormalComputer::testStdevMeasurementByMovingAverageOutdoor() {

}
