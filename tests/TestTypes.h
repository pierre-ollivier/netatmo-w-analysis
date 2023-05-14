#ifndef TESTTYPES_H
#define TESTTYPES_H

#include <QObject>

class TestTypes: public QObject
{
    Q_OBJECT

public:
    TestTypes();

private slots:
    void testDailyRecord();
    void testTimestampRecord();
};

#endif // TESTTYPES_H
