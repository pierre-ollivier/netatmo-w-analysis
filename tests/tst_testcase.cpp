#include <QtTest>
#include <QDate>
#include "../types/DailyRecord.cpp"
#include "../types/TimestampRecord.cpp"

class TestCase : public QObject
{
    Q_OBJECT

public:
    TestCase();

private slots:
    void testDailyRecord();
    void testTimestampRecord();
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

QTEST_APPLESS_MAIN(TestCase)

#include "tst_testcase.moc"
