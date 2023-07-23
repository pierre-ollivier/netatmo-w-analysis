#ifndef OLDDATAUPLOADER_H
#define OLDDATAUPLOADER_H

#include <QString>
#include <QObject>
#include <QDate>
#include "../backend/NetatmoAPIHandler.h"
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
    void setAccessToken(QString accessToken);

public slots:
    void addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor = false);
    void addExtTimestampRecordsFromCurrentMonth();
    void addExtTimestampRecordToCopyDatabase(ExtTimestampRecord);
    void addIntTimestampRecordToCopyDatabase(IntTimestampRecord);

    void logExtDailyRecord(ExtDailyRecord);
    void logIntDailyRecord(IntDailyRecord);
    void log3hRecords(QMap<QDate, std::tuple<double, double>> records);
    void logOutdoorTimestampRecords(QList<ExtTimestampRecord> records);

private:
    NetatmoAPIHandler *_apiHandler;
    QString _accessToken;
    QDate _beginDate = QDate();
    QDate _endDate = QDate();
    QMap<QDate, ExtendedExtDailyRecord *> extendedRecordsMap = QMap<QDate, ExtendedExtDailyRecord *>();
    int numberOfExtBatchesToRetrieve = 0;
    int numberOfIntBatchesToRetrieve = 0;
};

#endif // OLDDATAUPLOADER_H
