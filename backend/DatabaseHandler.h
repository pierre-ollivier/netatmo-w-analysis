#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>
#include <QSqlQuery>
#include "types/DailyRecord.h"
#include "types/TimestampRecord.h"

class DatabaseHandler
{
public:
    DatabaseHandler(QString pathToDatabase);

    void prepareQuery(QSqlQuery query, QString tableName, QString params[], int paramsSize);

    void postOutdoorDailyRecord(DailyRecord record, QString tableName);
    void postIndoorDailyRecord(DailyRecord record, QString tableName);
    void postOutdoorTimestampRecord(TimestampRecord record, QString tableName);
    void postIndoorTimestampRecord(TimestampRecord record, QString tableName);

private:
    QString _pathToDatabase;
};

#endif // DATABASEHANDLER_H
