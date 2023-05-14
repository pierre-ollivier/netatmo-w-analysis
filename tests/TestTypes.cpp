#include "TestTypes.h"
#include <QDate>
#include <QTest>
#include "../netatmo-w-analysis/types/DailyRecord.h"
#include "../netatmo-w-analysis/types/TimestampRecord.h"

TestTypes::TestTypes()
{

}

void TestTypes::testDailyRecord()
{
    DailyRecord r = DailyRecord(
            QDate(2023, 5, 7),
            0.0,
            0.0,
            0.0,
            0,
            0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            1000000000,
            1000000000,
            1000000000,
            1000000000,
            1000000000,
            1000000000,
            1000000000,
            1000000000
            );
    QCOMPARE(r.year(), 2023);
    QCOMPARE(r.month(), 5);
    QCOMPARE(r.day(), 7);
}

void TestTypes::testTimestampRecord() {
    TimestampRecord r = TimestampRecord(
                1600000000,
                20.5,
                60);
    QCOMPARE(r.date(), QDate(2020, 9, 13));
    QCOMPARE(r.time(), QTime(14, 26, 40));
    QCOMPARE(r.decade(), 26);
    QCOMPARE(r.weekNumber(), 37);
    QCOMPARE(int(r.dewPoint()), 12);
    QCOMPARE(int(r.humidex() + 0.5), 23);
}
