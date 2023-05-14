#include "TestTypes.h"
#include "TestDatabaseHandler.h"
#include "TestDailyStatisticsCalculator.h"
#include "TestDailyAverageCalculator.h"
#include <QtTest/QtTest>

int main(int argc, char *argv[])
{
    int status = 0;
    auto runTests = [&status, argc, argv](QObject* obj) {
        status |= QTest::qExec(obj, argc, argv);
    };

    runTests(new TestTypes);
    runTests(new TestDatabaseHandler);
    runTests(new TestDailyStatisticsCalculator);
    runTests(new TestDailyAverageCalculator);
    return status;
}
