#ifndef TESTTYPES_H
#define TESTTYPES_H

#include <QObject>

class TestTypes: public QObject
{
public:
    TestTypes();
    void testDailyRecord();
    void testTimestampRecord();
};

#endif // TESTTYPES_H
