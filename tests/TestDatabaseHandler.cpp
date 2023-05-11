#include "TestDatabaseHandler.h"
#include <QTest>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

TestDatabaseHandler::TestDatabaseHandler()
{

}

void TestDatabaseHandler::testGetTimestampRecordsFromDatabase() {
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

void TestDatabaseHandler::testGetDailyRecordsFromDatabase() {
    DatabaseHandler dbHandler("testdb.db");
    std::vector<IntDailyRecord> records = dbHandler.getIntDailyRecordsFromDatabase(
                "SELECT * from IndoorDailyRecords WHERE weekNumber = 41 ORDER BY id", -1);
    QCOMPARE(records.size(), 7);
    QCOMPARE(records[0].maxTemperature(), 21.8);
    QCOMPARE(records[1].minHumidity(), 58);
    QCOMPARE(records[2].maxCO2(),1219);
    QCOMPARE(records[3].maxNoise(), 51);
    std::vector<ExtDailyRecord> extRecords = dbHandler.getExtDailyRecordsFromDatabase(
                "SELECT * from OutdoorDailyRecords WHERE year = 2019 AND month = 10 ORDER BY day DESC", 1);
    QCOMPARE(extRecords.size(), 1);
    QCOMPARE(extRecords[0].minTemperature(), 9);
}
