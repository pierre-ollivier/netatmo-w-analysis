#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QDate>
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
    void postFromMultipleOutdoorCsv(QString path, QString tableName, QString beginMonth, QString endMonth);
    void postFromMultipleIndoorCsv(QString path, QString tableName, QString beginMonth, QString endMonth);

    std::vector<IntTimestampRecord> getIntTimestampRecordsFromDatabase(QString query, int N = 1);
    std::vector<ExtTimestampRecord> getExtTimestampRecordsFromDatabase(QString query, int N = 1);

    std::vector<IntDailyRecord> getIntDailyRecordsFromDatabase(QString query, int N = 1);
    std::vector<ExtDailyRecord> getExtDailyRecordsFromDatabase(QString query, int N = 1);

    QVariant getResultFromDatabase(QString query);

    void updateOutdoorDailyRecords(QDate beginDate, QDate endDate);
    void updateIndoorDailyRecords(QDate beginDate, QDate endDate);

private:
    QString _pathToDatabase;
};

#endif // DATABASEHANDLER_H
