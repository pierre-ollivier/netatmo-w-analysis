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
}

void OldDataUploader::addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor) {
    if (_accessToken == "") qDebug() << "Warning: undefined access token in OldDataUploader";
    int N = beginDate.daysTo(endDate);
    for (int n = 0; n < N - N % 3; n += 3) {
        long long beginTimestamp = QDateTime(beginDate.addDays(n)).toSecsSinceEpoch();
        long long endTimestamp = QDateTime(beginDate.addDays(n + 2)).toSecsSinceEpoch() + 86400;

        if (indoor) {
            _apiHandler->postIndoorDailyRequest(beginTimestamp, endTimestamp, "max", _accessToken);
        }
        else {
            _apiHandler->postOutdoorDailyRequest(beginTimestamp, endTimestamp, "max", _accessToken);
        }
    }

    long long beginTimestamp = QDateTime(beginDate.addDays(N - N % 3)).toSecsSinceEpoch();
    long long endTimestamp = QDateTime(beginDate.addDays(N)).toSecsSinceEpoch() + 86400;

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
