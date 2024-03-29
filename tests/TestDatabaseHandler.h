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
    void testGetResultFromDatabase();
    void testGetResultsFromDatabase();
    void testGetLatestDateTimeFromDatabase();
};

#endif // TESTDATABASEHANDLER_H
