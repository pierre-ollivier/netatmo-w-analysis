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
    connect(apiHandler, SIGNAL(extTimestampRecordRetrieved(ExtTimestampRecord)),
            SLOT(addExtTimestampRecordToCopyDatabase(ExtTimestampRecord)));
}

void OldDataUploader::addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor) {
    if (_accessToken == "") qDebug() << "Warning: undefined access token in OldDataUploader";
    long long beginTimestamp = QDateTime(beginDate).toSecsSinceEpoch();
    long long endTimestamp = QDateTime(endDate).toSecsSinceEpoch() + 86400;

    if (indoor) {
        _apiHandler->postIndoorDailyRequest(beginTimestamp, endTimestamp, "max", _accessToken);
    }
    else {
        _apiHandler->postOutdoorDailyRequest(beginTimestamp, endTimestamp, "max", _accessToken);
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
