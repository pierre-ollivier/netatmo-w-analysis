#include <QtTest>
#include <QDate>
#include "types/DailyRecord.h"

class TestCase : public QObject
{
    Q_OBJECT

public:
    TestCase();
    ~TestCase();

private slots:
    void testDailyRecord();

};

TestCase::TestCase()
{

}

TestCase::~TestCase()
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

QTEST_APPLESS_MAIN(TestCase)

#include "tst_testcase.moc"
