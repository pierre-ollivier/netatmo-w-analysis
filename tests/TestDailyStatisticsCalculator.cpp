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

}
void TestDailyStatisticsCalculator::testGetMaxDewPointTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetMinDewPointFromDate() {

}
void TestDailyStatisticsCalculator::testGetMinDewPointTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetAvgDewPointFromDate() {

}

// humidex

void TestDailyStatisticsCalculator::testGetMaxHumidexFromDate() {

}
void TestDailyStatisticsCalculator::testGetMaxHumidexTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetMinHumidexFromDate() {

}
void TestDailyStatisticsCalculator::testGetMinHumidexTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetAvgHumidexFromDate() {

}

// pressure

void TestDailyStatisticsCalculator::testGetMaxPressureFromDate() {

}
void TestDailyStatisticsCalculator::testGetMaxPressureTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetMinPressureFromDate() {

}
void TestDailyStatisticsCalculator::testGetMinPressureTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetAvgPressureFromDate() {

}

// co2

void TestDailyStatisticsCalculator::testGetMaxCO2FromDate() {

}
void TestDailyStatisticsCalculator::testGetMaxCO2TimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetMinCO2FromDate() {

}
void TestDailyStatisticsCalculator::testGetMinCO2TimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetAvgCO2FromDate() {

}

// noise

void TestDailyStatisticsCalculator::testGetMaxNoiseFromDate() {

}
void TestDailyStatisticsCalculator::testGetMaxNoiseTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetMinNoiseFromDate() {

}
void TestDailyStatisticsCalculator::testGetMinNoiseTimestampFromDate() {

}

void TestDailyStatisticsCalculator::testGetAvgNoiseFromDate() {

}

// others

void TestDailyStatisticsCalculator::testGetFirstTimestampFromDate() {

}
void TestDailyStatisticsCalculator::testGetFirstTimestampFromDateWithUTCOffset() {

}
