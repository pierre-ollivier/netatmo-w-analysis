#include "OldDataUploader.h"
#include <QDateTime>
#include "../backend/DatabaseHandler.h"

extern const QString PATH_TO_COPY_DATABASE;

OldDataUploader::OldDataUploader(NetatmoAPIHandler* apiHandler, QString accessToken)
{
    _apiHandler = apiHandler;
    _accessToken = accessToken;

    connect(apiHandler, SIGNAL(extTimestampRecordRetrieved(ExtTimestampRecord)),
            SLOT(addExtTimestampRecordToCopyDatabase(ExtTimestampRecord)));
    connect(apiHandler, SIGNAL(intTimestampRecordRetrieved(IntTimestampRecord)),
            SLOT(addIntTimestampRecordToCopyDatabase(IntTimestampRecord)));
    connect(apiHandler, SIGNAL(extDailyRecordRetrieved(ExtDailyRecord)), SLOT(logExtDailyRecord(ExtDailyRecord)));
}

void OldDataUploader::addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor) {
    qDebug() << beginDate << endDate;
    if (_accessToken == "") qDebug() << "Warning: undefined access token in OldDataUploader";
    _beginDate = beginDate; _endDate = endDate;

    long long beginTimestamp = QDateTime(beginDate).toSecsSinceEpoch();
    long long endTimestamp = QDateTime(endDate).toSecsSinceEpoch();
    if (!indoor) {
        _apiHandler->postFullOutdoorDailyRequest(beginTimestamp, endTimestamp, "1day", _accessToken);
    }
}


void OldDataUploader::addExtTimestampRecordToCopyDatabase(ExtTimestampRecord record) {
    DatabaseHandler dbHandlerCopy(PATH_TO_COPY_DATABASE);
    dbHandlerCopy.postOutdoorTimestampRecord(record, "OutdoorTimestampRecords");
}


void OldDataUploader::addIntTimestampRecordToCopyDatabase(IntTimestampRecord record) {
    DatabaseHandler dbHandlerCopy(PATH_TO_COPY_DATABASE);
    dbHandlerCopy.postIndoorTimestampRecord(record, "IndoorTimestampRecords");
}

void OldDataUploader::setAccessToken(QString accessToken) {
    _accessToken = accessToken;
}

void OldDataUploader::logExtDailyRecord(ExtDailyRecord record) {
    QString tableName = "OutdoorDailyRecords";
    DatabaseHandler dbHandler(PATH_TO_COPY_DATABASE);
    dbHandler.postOutdoorDailyRecord(record, tableName);
}
