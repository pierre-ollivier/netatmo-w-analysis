#include "TestTypes.h"

#include <QtTest/QtTest>

int main(int argc, char *argv[])
{
    int status = 0;
    auto runTest = [&status, argc, argv](QObject* obj) {
        status |= QTest::qExec(obj, argc, argv);
    };

    runTest(new TestTypes);
    return status;
}
