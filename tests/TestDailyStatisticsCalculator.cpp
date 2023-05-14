#include "TestDailyStatisticsCalculator.h"
#include <QTest>
#include <QDate>

TestDailyStatisticsCalculator::TestDailyStatisticsCalculator()
{
    calculator = new DailyStatisticsCalculator("testdb.db");
}

//temperature

void TestDailyStatisticsCalculator::testGetMaxTemperatureFromDate() {
    QCOMPARE(calculator->getMaxTemperatureFromDate(QDate(2019, 10, 6)), 17.8);
    QCOMPARE(calculator->getMaxTemperatureFromDate(QDate(2019, 10, 6), true), 21.7);
}
void TestDailyStatisticsCalculator::testGetMaxTemperatureTimestampFromDate() {
    QCOMPARE(calculator->getMaxTemperatureTimestampFromDate(QDate(2019, 10, 6)), 1570366928);
    QCOMPARE(calculator->getMaxTemperatureTimestampFromDate(QDate(2019, 10, 6), 17.8), 1570366928);
    QCOMPARE(calculator->getMaxTemperatureTimestampFromDate(QDate(2019, 10, 6), true), 1570376108);
    QCOMPARE(calculator->getMaxTemperatureTimestampFromDate(QDate(2019, 10, 6), 21.7, true), 1570376108);
}

void TestDailyStatisticsCalculator::testGetMinTemperatureFromDate() {
    QCOMPARE(calculator->getMinTemperatureFromDate(QDate(2019, 10, 7)), 13.2);
    QCOMPARE(calculator->getMinTemperatureFromDate(QDate(2019, 10, 7), true), 18.3);
}
void TestDailyStatisticsCalculator::testGetMinTemperatureTimestampFromDate() {
    QCOMPARE(calculator->getMinTemperatureTimestampFromDate(QDate(2019, 10, 7)), 1570428760);
    QCOMPARE(calculator->getMinTemperatureTimestampFromDate(QDate(2019, 10, 7), 13.2), 1570428760);
    QCOMPARE(calculator->getMinTemperatureTimestampFromDate(QDate(2019, 10, 7), true), 1570428185);
    QCOMPARE(calculator->getMinTemperatureTimestampFromDate(QDate(2019, 10, 7), 18.3, true), 1570428185);
}

void TestDailyStatisticsCalculator::testGetAvgTemperatureFromDate() {
    QCOMPARE(calculator->getAvgTemperatureFromDate(QDate(2019, 10, 8)), 14.4644872685185);
    QCOMPARE(calculator->getAvgTemperatureFromDate(QDate(2019, 10, 8), true), 20.8567164351852);
}

// humidity

void TestDailyStatisticsCalculator::testGetMaxHumidityFromDate() {
    QCOMPARE(calculator->getMaxHumidityFromDate(QDate(2019, 10, 9)), 95);
    QCOMPARE(calculator->getMaxHumidityFromDate(QDate(2019, 10, 9), true), 67);
}
void TestDailyStatisticsCalculator::testGetMaxHumidityTimestampFromDate() {
    QCOMPARE(calculator->getMaxHumidityTimestampFromDate(QDate(2019, 10, 9)), 1570635432);
    QCOMPARE(calculator->getMaxHumidityTimestampFromDate(QDate(2019, 10, 9), 95), 1570635432);
    QCOMPARE(calculator->getMaxHumidityTimestampFromDate(QDate(2019, 10, 9), true), 1570604900);
    QCOMPARE(calculator->getMaxHumidityTimestampFromDate(QDate(2019, 10, 9), 67, true), 1570604900);
}

void TestDailyStatisticsCalculator::testGetMinHumidityFromDate() {
    QCOMPARE(calculator->getMinHumidityFromDate(QDate(2019, 10, 10)), 75);
    QCOMPARE(calculator->getMinHumidityFromDate(QDate(2019, 10, 10), true), 56);
}
void TestDailyStatisticsCalculator::testGetMinHumidityTimestampFromDate() {
    QCOMPARE(calculator->getMinHumidityTimestampFromDate(QDate(2019, 10, 10)), 1570717157);
    QCOMPARE(calculator->getMinHumidityTimestampFromDate(QDate(2019, 10, 10), 75), 1570717157);
    QCOMPARE(calculator->getMinHumidityTimestampFromDate(QDate(2019, 10, 10), true), 1570701134);
    QCOMPARE(calculator->getMinHumidityTimestampFromDate(QDate(2019, 10, 10), 56, true), 1570701134);
}

void TestDailyStatisticsCalculator::testGetAvgHumidityFromDate() {
    QCOMPARE(calculator->getAvgHumidityFromDate(QDate(2019, 10, 11)), 83.1311111111111);
    QCOMPARE(calculator->getAvgHumidityFromDate(QDate(2019, 10, 11), true), 60.3709490740741);
}

// dew point

void TestDailyStatisticsCalculator::testGetMaxDewPointFromDate() {
    QCOMPARE(calculator->getMaxDewPointFromDate(QDate(2019, 10, 12)), 15.564);
    QCOMPARE(calculator->getMaxDewPointFromDate(QDate(2019, 10, 12), true), 16.18);
}
void TestDailyStatisticsCalculator::testGetMaxDewPointTimestampFromDate() {
    QCOMPARE(calculator->getMaxDewPointTimestampFromDate(QDate(2019, 10, 12)), 1570896877);
    QCOMPARE(calculator->getMaxDewPointTimestampFromDate(QDate(2019, 10, 12), 15.564), 1570896877);
    QCOMPARE(calculator->getMaxDewPointTimestampFromDate(QDate(2019, 10, 12), true), 1570911815);
    QCOMPARE(calculator->getMaxDewPointTimestampFromDate(QDate(2019, 10, 12), 16.18, true), 1570911815);
}

void TestDailyStatisticsCalculator::testGetMinDewPointFromDate() {
    QCOMPARE(calculator->getMinDewPointFromDate(QDate(2019, 10, 13)), 12.697);
    QCOMPARE(calculator->getMinDewPointFromDate(QDate(2019, 10, 13), true), 14.396);
}
void TestDailyStatisticsCalculator::testGetMinDewPointTimestampFromDate() {
    QCOMPARE(calculator->getMinDewPointTimestampFromDate(QDate(2019, 10, 13)), 1571003461);
    QCOMPARE(calculator->getMinDewPointTimestampFromDate(QDate(2019, 10, 13), 12.697), 1571003461);
    QCOMPARE(calculator->getMinDewPointTimestampFromDate(QDate(2019, 10, 13), true), 1570950713);
    QCOMPARE(calculator->getMinDewPointTimestampFromDate(QDate(2019, 10, 13), 14.396, true), 1570950713);
}

void TestDailyStatisticsCalculator::testGetAvgDewPointFromDate() {
    QCOMPARE(calculator->getAvgDewPointFromDate(QDate(2019, 10, 14)), 13.4179805370356);
    QCOMPARE(calculator->getAvgDewPointFromDate(QDate(2019, 10, 14), true), 14.9087712649165);
}

// humidex

void TestDailyStatisticsCalculator::testGetMaxHumidexFromDate() {
    QCOMPARE(calculator->getMaxHumidexFromDate(QDate(2019, 10, 15)), 20.151);
    QCOMPARE(calculator->getMaxHumidexFromDate(QDate(2019, 10, 15), true), 26.833);
}
void TestDailyStatisticsCalculator::testGetMaxHumidexTimestampFromDate() {
    QCOMPARE(calculator->getMaxHumidexTimestampFromDate(QDate(2019, 10, 15)), 1571090620);
    QCOMPARE(calculator->getMaxHumidexTimestampFromDate(QDate(2019, 10, 15), 20.151), 1571090620);
    QCOMPARE(calculator->getMaxHumidexTimestampFromDate(QDate(2019, 10, 15), true), 1571091259);
    QCOMPARE(calculator->getMaxHumidexTimestampFromDate(QDate(2019, 10, 15), 26.833, true), 1571091259);
}

void TestDailyStatisticsCalculator::testGetMinHumidexFromDate() {
    QCOMPARE(calculator->getMinHumidexFromDate(QDate(2019, 10, 16)), 13.581);
    QCOMPARE(calculator->getMinHumidexFromDate(QDate(2019, 10, 16), true), 21.941);
}
void TestDailyStatisticsCalculator::testGetMinHumidexTimestampFromDate() {
    QCOMPARE(calculator->getMinHumidexTimestampFromDate(QDate(2019, 10, 16)), 1571180754);
    QCOMPARE(calculator->getMinHumidexTimestampFromDate(QDate(2019, 10, 16), 13.581), 1571180754);
    QCOMPARE(calculator->getMinHumidexTimestampFromDate(QDate(2019, 10, 16), true), 1571207405);
    QCOMPARE(calculator->getMinHumidexTimestampFromDate(QDate(2019, 10, 16), 21.941, true), 1571207405);
}

void TestDailyStatisticsCalculator::testGetAvgHumidexFromDate() {
    QCOMPARE(calculator->getAvgHumidexFromDate(QDate(2019, 10, 17)), 17.4257239310958);
    QCOMPARE(calculator->getAvgHumidexFromDate(QDate(2019, 10, 17), true), 24.2586105458907);
}

// pressure

void TestDailyStatisticsCalculator::testGetMaxPressureFromDate() {
    QCOMPARE(calculator->getMaxPressureFromDate(QDate(2019, 10, 18)), 1010.3);
}
void TestDailyStatisticsCalculator::testGetMaxPressureTimestampFromDate() {
    QCOMPARE(calculator->getMaxPressureTimestampFromDate(QDate(2019, 10, 18)), 1571394921);
    QCOMPARE(calculator->getMaxPressureTimestampFromDate(QDate(2019, 10, 18), 1010.3), 1571394921);
}

void TestDailyStatisticsCalculator::testGetMinPressureFromDate() {
    QCOMPARE(calculator->getMinPressureFromDate(QDate(2019, 10, 19)), 1005);
}
void TestDailyStatisticsCalculator::testGetMinPressureTimestampFromDate() {
    QCOMPARE(calculator->getMinPressureTimestampFromDate(QDate(2019, 10, 19)), 1571459014);
    QCOMPARE(calculator->getMinPressureTimestampFromDate(QDate(2019, 10, 19), 1005), 1571459014);
}

void TestDailyStatisticsCalculator::testGetAvgPressureFromDate() {
    QCOMPARE(calculator->getAvgPressureFromDate(QDate(2019, 10, 20)), 1006.85592361111);
}

// co2

void TestDailyStatisticsCalculator::testGetMaxCO2FromDate() {
    QCOMPARE(calculator->getMaxCO2FromDate(QDate(2019, 10, 21)), 431);
}

void TestDailyStatisticsCalculator::testGetMinCO2FromDate() {
    QCOMPARE(calculator->getMinCO2FromDate(QDate(2019, 10, 22)), 362);
}

void TestDailyStatisticsCalculator::testGetAvgCO2FromDate() {
    QCOMPARE(calculator->getAvgCO2FromDate(QDate(2019, 10, 23)), 376.909618055556);
}

// noise

void TestDailyStatisticsCalculator::testGetMaxNoiseFromDate() {
    QCOMPARE(calculator->getMaxNoiseFromDate(QDate(2019, 10, 25)), 62);
}

void TestDailyStatisticsCalculator::testGetMinNoiseFromDate() {
    QCOMPARE(calculator->getMinNoiseFromDate(QDate(2019, 10, 24)), 36);
}

void TestDailyStatisticsCalculator::testGetAvgNoiseFromDate() {
    QCOMPARE(calculator->getAvgNoiseFromDate(QDate(2019, 10, 26)), 38.8109027777778);
}

// others

void TestDailyStatisticsCalculator::testGetFirstTimestampFromDate() {
    QCOMPARE(calculator->getFirstTimestampFromDate(QDate(2019, 10, 27)), 1572127200);  // midnight in summer time
    QCOMPARE(calculator->getFirstTimestampFromDate(QDate(2019, 10, 28)), 1572217200);  // midnight in winter time
}
void TestDailyStatisticsCalculator::testGetFirstTimestampFromDateWithUTCOffset() {
    QCOMPARE(calculator->getFirstTimestampFromDateWithUTCOffset(QDate(2019, 10, 29), 1), 1572310800);  // 29/10/2019 01:00 UTC
}
