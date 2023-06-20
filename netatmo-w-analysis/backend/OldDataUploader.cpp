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
    connect(apiHandler, SIGNAL(intDailyRecordRetrieved(IntDailyRecord)), SLOT(logIntDailyRecord(IntDailyRecord)));
    connect(apiHandler,
            SIGNAL(ext3hRecordsRetrieved(QMap<QDate, std::tuple<double, double>>)),
            SLOT(log3hRecords(QMap<QDate, std::tuple<double, double>>)));
}

void OldDataUploader::addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor) {
    if (_accessToken == "") qDebug() << "Warning: undefined access token in OldDataUploader";
    _beginDate = beginDate; _endDate = endDate;

    long long beginTimestamp = QDateTime(beginDate).toSecsSinceEpoch();
    long long endTimestamp = QDateTime(endDate).toSecsSinceEpoch();
    if (indoor) {
        _apiHandler->postFullIndoorDailyRequest(beginTimestamp, endTimestamp, "1day", _accessToken);
    }
    else {
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
    QDate date = record.date();
    if (extendedRecordsMap.contains(date)) {
        extendedRecordsMap[date].setExtDailyRecord(record);
        dbHandler.postOutdoorDailyRecord(extendedRecordsMap[date].extDailyRecord(), tableName);
    }
    else {
        extendedRecordsMap.insert(date, ExtendedExtDailyRecord());
        extendedRecordsMap[date].setExtDailyRecord(record);
    }
}

void OldDataUploader::logIntDailyRecord(IntDailyRecord record) {
    QString tableName = "IndoorDailyRecords";
    DatabaseHandler dbHandler(PATH_TO_COPY_DATABASE);
    dbHandler.postIndoorDailyRecord(record, tableName);
}

void OldDataUploader::log3hRecords(QMap<QDate, std::tuple<double, double>> records) {
    QString tableName = "OutdoorDailyRecords";
    DatabaseHandler dbHandler(PATH_TO_COPY_DATABASE);
    for (QDate date : records.keys()) {
        if (extendedRecordsMap.contains(date)) {
            extendedRecordsMap[date].setMinTemperature(std::get<0>(records[date]));
            extendedRecordsMap[date].setMaxTemperature(std::get<1>(records[date]));
            dbHandler.postOutdoorDailyRecord(extendedRecordsMap[date].extDailyRecord(), tableName);
        }
        else {
            extendedRecordsMap.insert(date, ExtendedExtDailyRecord());
            extendedRecordsMap[date].setMinTemperature(std::get<0>(records[date]));
            extendedRecordsMap[date].setMaxTemperature(std::get<1>(records[date]));
        }
    }
}
