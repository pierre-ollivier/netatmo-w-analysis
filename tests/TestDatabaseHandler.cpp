#include "TestDatabaseHandler.h"
#include <QTest>
#include "../netatmo-w-analysis/types/IntTimestampRecord.h"
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
