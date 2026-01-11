#ifndef OLDDATAUPLOADER_H
#define OLDDATAUPLOADER_H

#include <QString>
#include <QObject>
#include <QDate>
#include "../backend/DatabaseHandler.h"
#include "../backend/NetatmoAPIHandler.h"
#include "../backend/DailyStatisticsCalculator.h"
#include "../types/ExtTimestampRecord.h"
#include "../types/IntTimestampRecord.h"
#include "../types/ExtDailyRecord.h"
#include "../types/IntDailyRecord.h"
#include "../types/ExtendedExtDailyRecord.h"

class OldDataUploader : public QObject
{
    Q_OBJECT

public:
    OldDataUploader(NetatmoAPIHandler *apiHandler, QString accessToken = "");
    OldDataUploader(QObject *parent, NetatmoAPIHandler *apiHandler, QString accessToken = "");
    void setAccessToken(QString accessToken);

signals:
    void outdoorTimestampRecordsLogged();
    void indoorTimestampRecordsLogged();

public slots:
    void addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor = false);

    void addAllExtTimestampRecordsFromPeriod(QDate beginDate, QDate endDate);
    void addAllIntTimestampRecordsFromPeriod(QDate beginDate, QDate endDate);

    void addExtTimestampRecordsFromCurrentMonth();
    void addIntTimestampRecordsFromCurrentMonth();
    void addExtTimestampRecordToCopyDatabase(ExtTimestampRecord);
    void addIntTimestampRecordToCopyDatabase(IntTimestampRecord);
    void addBackfillExtRecords(QDate beginDate, QDate endDate, QList<ExtTimestampRecord> records);
    void addBackfillIntRecords(QDate beginDate, QDate endDate, QList<IntTimestampRecord> records);

    void logExtDailyRecord(ExtDailyRecord);
    void logIntDailyRecord(IntDailyRecord);
    void log3hRecords(QMap<QDate, std::tuple<double, double>> records);
    void logOutdoorTimestampRecords(QList<ExtTimestampRecord> records);
    void logIndoorTimestampRecords(QList<IntTimestampRecord> records);

private:
    NetatmoAPIHandler *_apiHandler;
    QString _accessToken;
    QDate _beginDate = QDate();
    QDate _endDate = QDate();
    QMap<QDate, ExtendedExtDailyRecord *> extendedRecordsMap = QMap<QDate, ExtendedExtDailyRecord *>();
    DatabaseHandler *dbHandlerProd;
    DatabaseHandler *dbHandlerCopy;
    DailyStatisticsCalculator *_dailyCalculator;
    int numberOfExtBatchesToRetrieve = 0;
    int numberOfIntBatchesToRetrieve = 0;
};

#endif // OLDDATAUPLOADER_H
