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
    connect(apiHandler, SIGNAL(endOfDailyIndoorConditionsRetrieval()), SLOT(intBatchRetrieved()));
    connect(apiHandler, SIGNAL(endOfDailyOutdoorConditionsRetrieval()), SLOT(extBatchRetrieved()));
    connect(apiHandler, SIGNAL(extDailyRecordRetrieved(ExtDailyRecord)), SLOT(logExtDailyRecord(ExtDailyRecord)));
}

void OldDataUploader::addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor) {
    qDebug() << beginDate << endDate;
    if (_accessToken == "") qDebug() << "Warning: undefined access token in OldDataUploader";
    int N = beginDate.daysTo(endDate);
    _beginDate = beginDate; _endDate = endDate;

//    for (int n = 0; n <= N; n += 3) {
//        if (indoor) numberOfIntBatchesToRetrieve += 1; else numberOfExtBatchesToRetrieve += 1;

//        long long beginTimestamp = QDateTime(beginDate.addDays(n)).toSecsSinceEpoch();
//        long long endTimestamp = QDateTime(beginDate.addDays(n + 2)).toSecsSinceEpoch() + 86400;

//        if (n == 0) { // first day: we add the hours of the previous day after 18 UTC
//            beginTimestamp = QDateTime(beginDate.addDays(-1), QTime(18, 0), Qt::UTC).toSecsSinceEpoch();
//        }
//        if (n + 3 > N) { //last day: we add the hours of the next day before 6 UTC
//            endTimestamp = QDateTime(endDate.addDays(1), QTime(6, 0), Qt::UTC).toSecsSinceEpoch();
//        }

//        if (indoor) {
//            _apiHandler->postIndoorDailyRequest(beginTimestamp, endTimestamp, "max", _accessToken);
//        }
//        else {
//            _apiHandler->postOutdoorDailyRequest(beginTimestamp, endTimestamp, "max", _accessToken);
//        }
//    }

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

void OldDataUploader::extBatchRetrieved() {
    DatabaseHandler dbHandlerCopy(PATH_TO_COPY_DATABASE);
    numberOfExtBatchesToRetrieve--;
    if (numberOfExtBatchesToRetrieve == 0) {
//        dbHandlerCopy.updateOutdoorDailyRecords(_beginDate, _endDate, false);
    }
}

void OldDataUploader::intBatchRetrieved() {
    DatabaseHandler dbHandlerCopy(PATH_TO_COPY_DATABASE);
    numberOfIntBatchesToRetrieve--;
    qDebug() << numberOfIntBatchesToRetrieve;
    if (numberOfIntBatchesToRetrieve == 0) {
//        dbHandlerCopy.updateIndoorDailyRecords(_beginDate, _endDate, false);
    }
}

void OldDataUploader::setAccessToken(QString accessToken) {
    _accessToken = accessToken;
}

void OldDataUploader::logExtDailyRecord(ExtDailyRecord record) {
    QString tableName = "OutdoorDailyRecords";
    DatabaseHandler dbHandler(PATH_TO_COPY_DATABASE);
    dbHandler.postOutdoorDailyRecord(record, PATH_TO_COPY_DATABASE);
}
