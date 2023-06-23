#include "TestNormalComputer.h"
#include <QTest>
#include <math.h>
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
    NormalComputer normalComputer(new DatabaseHandler("testdb.db")); // TODO: change this
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "minTemperature",
                 5), (12.9 + 13.6 + 13.6 + 14.7 + 15.2 + 5.8 + 7.4 + 8.5 + 5.2 + 4) / 10);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "maxTemperature",
                 5), 19.55);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "minHumidity",
                 5), 36.3);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "maxHumidity",
                 5), 70.1);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "minDewPoint",
                 5), 1.8063);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "maxDewPoint",
                 5), 7.334);

    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "minHumidex",
                 5), 9.0436);
    QCOMPARE(normalComputer.normalMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "maxHumidex",
                 5), 19.1757);
}

void TestNormalComputer::testStdevMeasurementByMovingAverageIndoor() {
    NormalComputer normalComputer(new DatabaseHandler("testdb.db")); // TODO: change this
    QCOMPARE(normalComputer.stdevMeasurementByMovingAverage(
                 "IndoorDailyRecords",
                 QDate(2023, 10, 10),
                 "minHumidity",
                 5), std::sqrt((0.04 + 0.04 + 4.84 + 0.64 + 3.24) / 5));
}

void TestNormalComputer::testStdevMeasurementByMovingAverageOutdoor() {
    NormalComputer normalComputer(new DatabaseHandler("testdb.db")); // TODO: change this
    QCOMPARE(normalComputer.stdevMeasurementByMovingAverage(
                 "OutdoorDailyRecords",
                 QDate(2023, 4, 10),
                 "maxHumidity",
                 5), std::sqrt((65.61 + 123.21 + 50.41 + 102.01 + 259.21
                                + 37.21 + 118.81 + 436.81 + 436.81 + 34.81) / 10));
}
