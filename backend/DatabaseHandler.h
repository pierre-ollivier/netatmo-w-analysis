#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>
#include "types/DailyRecord.h"
#include "types/TimestampRecord.h"

class DatabaseHandler
{
public:
    DatabaseHandler(QString pathToDatabase);
    void postOutdoorDailyRecord(DailyRecord record, QString tableName);
    void postOutdoorTimestampRecord(TimestampRecord record, QString tableName);

private:
    QString _pathToDatabase;
};

#endif // DATABASEHANDLER_H
