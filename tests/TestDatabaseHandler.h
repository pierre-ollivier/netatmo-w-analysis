#ifndef TESTDATABASEHANDLER_H
#define TESTDATABASEHANDLER_H

#include <QObject>

class TestDatabaseHandler : public QObject
{
    Q_OBJECT

public:
    TestDatabaseHandler();

private slots:
    void testGetTimestampRecordsFromDatabase();
    void testGetDailyRecordsFromDatabase();
};

#endif // TESTDATABASEHANDLER_H
