#include <QtTest>
#include <QDate>
#include "../netatmo-w-analysis/types/DailyRecord.h"
#include "../netatmo-w-analysis/types/IntDailyRecord.h"
#include "../netatmo-w-analysis/types/ExtDailyRecord.h"
#include "../netatmo-w-analysis/types/TimestampRecord.h"
#include "../netatmo-w-analysis/types/IntTimestampRecord.h"
#include "../netatmo-w-analysis/types/ExtTimestampRecord.h"
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"
#include "../netatmo-w-analysis/backend/DailyStatisticsCalculator.h"

class TestCase : public QObject
{
    Q_OBJECT

public:
    TestCase();

private slots:
    void testDailyRecord();
    void testTimestampRecord();
    void testGetTimestampRecordsFromDatabase();
};

TestCase::TestCase()
{

}

void TestCase::testDailyRecord()
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

void TestCase::testTimestampRecord() {
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

void TestCase::testGetTimestampRecordsFromDatabase() {
    DatabaseHandler dbHandler("testdb.db");
    std::vector<IntTimestampRecord> records = dbHandler.getIntTimestampRecordsFromDatabase(
                "SELECT * from IndoorTimestampRecords WHERE date = \"05/10/2019\" ORDER BY id", 2);
    QCOMPARE(records[0].temperature(), 19.8);
    QCOMPARE(records[1].temperature(), 19.7);
    std::vector<ExtTimestampRecord> extRecords = dbHandler.getExtTimestampRecordsFromDatabase(
                "SELECT * from OutdoorTimestampRecords WHERE date = \"05/10/2019\" ORDER BY id", 3);
    QCOMPARE(extRecords[0].temperature(), 17.2);
    QCOMPARE(extRecords[1].temperature(), 17.2);
    QCOMPARE(extRecords[2].temperature(), 17.3);
}

QTEST_APPLESS_MAIN(TestCase)

#include "tst_testcase.moc"
