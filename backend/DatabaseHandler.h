#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>
#include <QSqlQuery>
#include "types/IntDailyRecord.h"
#include "types/ExtDailyRecord.h"
#include "types/ExtTimestampRecord.h"
#include "types/IntTimestampRecord.h"

class DatabaseHandler
{
public:
    DatabaseHandler(QString pathToDatabase);

    void prepareQuery(QSqlQuery query, QString tableName, QString params[], int paramsSize);

    void postOutdoorDailyRecord(ExtDailyRecord record, QString tableName);
    void postIndoorDailyRecord(IntDailyRecord record, QString tableName);
    void postOutdoorTimestampRecord(ExtTimestampRecord record, QString tableName);
    void postIndoorTimestampRecord(IntTimestampRecord record, QString tableName);

    void postFromOutdoorCsv(QString pathToCsv, QString tableName, QDate beginDate = QDate(), QDate endDate = QDate());
    void postFromIndoorCsv(QString pathToCsv, QString tableName, QDate beginDate = QDate(), QDate endDate = QDate());

    std::vector<IntTimestampRecord> getIntTimestampRecordsFromDatabase(QString tableName, QString query, int N = 1);

private:
    QString _pathToDatabase;
};

#endif // DATABASEHANDLER_H
