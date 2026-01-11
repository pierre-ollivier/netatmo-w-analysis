#include "OldDataUploader.h"
#include <QDateTime>

extern const QString PATH_TO_COPY_DATABASE;
extern const QString PATH_TO_PROD_DATABASE;

OldDataUploader::OldDataUploader(NetatmoAPIHandler* apiHandler, QString accessToken)
{
    _apiHandler = apiHandler;
    _accessToken = accessToken;

    dbHandlerProd = new DatabaseHandler(this, PATH_TO_PROD_DATABASE);
    dbHandlerCopy = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);
    _dailyCalculator = new DailyStatisticsCalculator(this, PATH_TO_COPY_DATABASE, dbHandlerCopy);

    connect(apiHandler, SIGNAL(extTimestampRecordRetrieved(ExtTimestampRecord)),
            SLOT(addExtTimestampRecordToCopyDatabase(ExtTimestampRecord)));
    connect(apiHandler, SIGNAL(intTimestampRecordRetrieved(IntTimestampRecord)),
            SLOT(addIntTimestampRecordToCopyDatabase(IntTimestampRecord)));
    connect(apiHandler, SIGNAL(extDailyRecordRetrieved(ExtDailyRecord)), SLOT(logExtDailyRecord(ExtDailyRecord)));
    connect(apiHandler, SIGNAL(intDailyRecordRetrieved(IntDailyRecord)), SLOT(logIntDailyRecord(IntDailyRecord)));
    connect(apiHandler,
            SIGNAL(ext3hRecordsRetrieved(QMap<QDate,std::tuple<double,double> >)),
            SLOT(log3hRecords(QMap<QDate,std::tuple<double,double> >)));
}

OldDataUploader::OldDataUploader(QObject *parent, NetatmoAPIHandler* apiHandler, QString accessToken) : QObject(parent)
{
    _apiHandler = apiHandler;
    _accessToken = accessToken;

    dbHandlerProd = new DatabaseHandler(this, PATH_TO_PROD_DATABASE);
    dbHandlerCopy = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);
    _dailyCalculator = new DailyStatisticsCalculator(this, PATH_TO_COPY_DATABASE, dbHandlerCopy);

    connect(apiHandler, SIGNAL(extTimestampRecordRetrieved(ExtTimestampRecord)),
            SLOT(addExtTimestampRecordToCopyDatabase(ExtTimestampRecord)));
    connect(apiHandler, SIGNAL(intTimestampRecordRetrieved(IntTimestampRecord)),
            SLOT(addIntTimestampRecordToCopyDatabase(IntTimestampRecord)));
    connect(apiHandler, SIGNAL(extDailyRecordRetrieved(ExtDailyRecord)), SLOT(logExtDailyRecord(ExtDailyRecord)));
    connect(apiHandler, SIGNAL(intDailyRecordRetrieved(IntDailyRecord)), SLOT(logIntDailyRecord(IntDailyRecord)));
    connect(apiHandler,
            SIGNAL(ext3hRecordsRetrieved(QMap<QDate,std::tuple<double,double> >)),
            SLOT(log3hRecords(QMap<QDate,std::tuple<double,double> >)));
}

void OldDataUploader::addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor) {
    if (_accessToken == "") qDebug() << "Warning: undefined access token in OldDataUploader";
    _beginDate = beginDate; _endDate = endDate;

    long long beginTimestamp = QDateTime(beginDate, QTime(0, 0)).toSecsSinceEpoch();
    long long endTimestamp = QDateTime(endDate, QTime(0, 0)).toSecsSinceEpoch();
    if (indoor) {
        _apiHandler->postFullIndoorDailyRequest(beginTimestamp, endTimestamp, "1day", _accessToken);
    }
    else {
        beginTimestamp = QDateTime(beginDate.addDays(-1), QTime(18, 0), Qt::UTC).toSecsSinceEpoch();
        endTimestamp = QDateTime(endDate.addDays(0), QTime(6, 0), Qt::UTC).toSecsSinceEpoch();
        _apiHandler->postFullOutdoorDailyRequest(beginTimestamp, endTimestamp, "1day", _accessToken);
        _apiHandler->post3hDailyRequest(beginTimestamp, endTimestamp, _accessToken);
    }
}

void OldDataUploader::addExtTimestampRecordsFromCurrentMonth() {
    NetatmoAPIHandler *apiHandler = new NetatmoAPIHandler(this, _apiHandler->getAPIMonitor());
    connect(apiHandler,
            SIGNAL(outdoorRecordListRetrieved(QList<ExtTimestampRecord>)),
            SLOT(logOutdoorTimestampRecords(QList<ExtTimestampRecord>)));
    QDateTime dt = QDateTime(QDate::currentDate().addDays(-1), QTime(0, 0));
    apiHandler->postOutdoorTimestampRecordsRequest(dt.toSecsSinceEpoch(),
                                                   QDateTime::currentSecsSinceEpoch(),
                                                   _accessToken);
}

void OldDataUploader::addAllExtTimestampRecordsFromPeriod(QDate beginDate, QDate endDate) {
    NetatmoAPIHandler *apiHandler = new NetatmoAPIHandler(this, _apiHandler->getAPIMonitor());
    connect(apiHandler,
            SIGNAL(outdoorMultiDaysRecordListRetrieved(QDate, QDate, QList<ExtTimestampRecord>)),
            SLOT(addBackfillExtRecords(QDate, QDate, QList<ExtTimestampRecord>))
            );

    apiHandler->postMultiDaysOutdoorTimestampRecordsRequest(beginDate, endDate, _accessToken);
}

void OldDataUploader::addAllIntTimestampRecordsFromPeriod(QDate beginDate, QDate endDate) {
    NetatmoAPIHandler *apiHandler = new NetatmoAPIHandler(this, _apiHandler->getAPIMonitor());
    connect(apiHandler,
            SIGNAL(indoorMultiDaysRecordListRetrieved(QDate, QDate, QList<IntTimestampRecord>)),
            SLOT(addBackfillIntRecords(QDate, QDate, QList<IntTimestampRecord>))
            );

    apiHandler->postMultiDaysIndoorTimestampRecordsRequest(beginDate, endDate, _accessToken);
}

void OldDataUploader::addIntTimestampRecordsFromCurrentMonth() {
    NetatmoAPIHandler *apiHandler = new NetatmoAPIHandler(this, _apiHandler->getAPIMonitor());
    connect(apiHandler,
            SIGNAL(indoorRecordListRetrieved(QList<IntTimestampRecord>)),
            SLOT(logIndoorTimestampRecords(QList<IntTimestampRecord>)));
    QDateTime dt = QDateTime(QDate::currentDate().addDays(-1), QTime(0, 0));
    apiHandler->postIndoorTimestampRecordsRequest(dt.toSecsSinceEpoch(),
                                                  QDateTime::currentSecsSinceEpoch(),
                                                  _accessToken);
}


void OldDataUploader::addExtTimestampRecordToCopyDatabase(ExtTimestampRecord record) {
    dbHandlerCopy->postOutdoorTimestampRecord(record, "OutdoorTimestampRecords");
}


void OldDataUploader::addIntTimestampRecordToCopyDatabase(IntTimestampRecord record) {
    dbHandlerCopy->postIndoorTimestampRecord(record, "IndoorTimestampRecords");
}

void OldDataUploader::addBackfillExtRecords(QDate beginDate, QDate endDate, QList<ExtTimestampRecord> records) {
    for (ExtTimestampRecord record : records) {
        if (record.date() >= beginDate && record.date() <= endDate) {
            dbHandlerProd->postOutdoorTimestampRecord(record, "OutdoorTimestampRecords");
            dbHandlerCopy->postOutdoorTimestampRecord(record, "OutdoorTimestampRecords");
        }
    }
    for (QDate d = beginDate; d <= endDate; d = d.addDays(1)) {

        QPair<QVariant, long long> maxTemperature = _dailyCalculator->getMaxOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.temperature();}, false, 6);
        QPair<QVariant, long long> minTemperature = _dailyCalculator->getMinOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.temperature();}, false, -6);
        QPair<QVariant, long long> maxHumidity = _dailyCalculator->getMaxOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.humidity();});
        QPair<QVariant, long long> minHumidity = _dailyCalculator->getMinOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.humidity();});
        QPair<QVariant, long long> maxDewPoint = _dailyCalculator->getMaxOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.dewPoint();});
        QPair<QVariant, long long> minDewPoint = _dailyCalculator->getMinOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.dewPoint();});
        QPair<QVariant, long long> maxHumidex = _dailyCalculator->getMaxOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.humidex();});
        QPair<QVariant, long long> minHumidex = _dailyCalculator->getMinOutdoorMeasurementInfoFromDate(
            d, records, [](ExtTimestampRecord record) {return record.humidex();});

        double avgTemperature = _dailyCalculator->getAvgOutdoorMeasurementFromDate(d, records, "temperature");
        double avgHumidity = _dailyCalculator->getAvgOutdoorMeasurementFromDate(d, records, "humidity");
        double avgDewPoint = _dailyCalculator->getAvgOutdoorMeasurementFromDate(d, records, "dewPoint");
        double avgHumidex = _dailyCalculator->getAvgOutdoorMeasurementFromDate(d, records, "humidex");

        ExtDailyRecord record = ExtDailyRecord(
            d,
            maxTemperature.first.toDouble(),
            minTemperature.first.toDouble(),
            avgTemperature,
            maxHumidity.first.toInt(),
            minHumidity.first.toInt(),
            avgHumidity,
            maxDewPoint.first.toDouble(),
            minDewPoint.first.toDouble(),
            avgDewPoint,
            maxHumidex.first.toDouble(),
            minHumidex.first.toDouble(),
            avgHumidex,
            maxTemperature.second,
            minTemperature.second,
            maxHumidity.second,
            minHumidity.second,
            maxDewPoint.second,
            minDewPoint.second,
            maxHumidex.second,
            minHumidex.second
            );
        dbHandlerProd->postOutdoorDailyRecord(record, "OutdoorDailyRecords");
        dbHandlerCopy->postOutdoorDailyRecord(record, "OutdoorDailyRecords");
    }
}

void OldDataUploader::addBackfillIntRecords(QDate beginDate, QDate endDate, QList<IntTimestampRecord> records) {
    for (IntTimestampRecord record : records) {
        if (record.date() >= beginDate && record.date() <= endDate) {
            dbHandlerProd->postIndoorTimestampRecord(record, "IndoorTimestampRecords");
            dbHandlerCopy->postIndoorTimestampRecord(record, "IndoorTimestampRecords");
        }
    }
    for (QDate d = beginDate; d <= endDate; d = d.addDays(1)) {

        QPair<QVariant, long long> maxTemperature = _dailyCalculator->getMaxIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.temperature();}, false, 6);
        QPair<QVariant, long long> minTemperature = _dailyCalculator->getMinIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.temperature();}, false, -6);
        QPair<QVariant, long long> maxHumidity = _dailyCalculator->getMaxIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.humidity();});
        QPair<QVariant, long long> minHumidity = _dailyCalculator->getMinIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.humidity();});
        QPair<QVariant, long long> maxDewPoint = _dailyCalculator->getMaxIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.dewPoint();});
        QPair<QVariant, long long> minDewPoint = _dailyCalculator->getMinIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.dewPoint();});
        QPair<QVariant, long long> maxHumidex = _dailyCalculator->getMaxIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.humidex();});
        QPair<QVariant, long long> minHumidex = _dailyCalculator->getMinIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.humidex();});
        QPair<QVariant, long long> maxPressure = _dailyCalculator->getMaxIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.pressure();});
        QPair<QVariant, long long> minPressure = _dailyCalculator->getMinIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.pressure();});
        QPair<QVariant, long long> maxCO2 = _dailyCalculator->getMaxIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.co2();});
        QPair<QVariant, long long> minCO2 = _dailyCalculator->getMinIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.co2();});
        QPair<QVariant, long long> maxNoise = _dailyCalculator->getMaxIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.noise();});
        QPair<QVariant, long long> minNoise = _dailyCalculator->getMinIndoorMeasurementInfoFromDate(
            d, records, [](IntTimestampRecord record) {return record.noise();});

        double avgTemperature = _dailyCalculator->getAvgIndoorMeasurementFromDate(d, records, "temperature");
        double avgHumidity = _dailyCalculator->getAvgIndoorMeasurementFromDate(d, records, "humidity");
        double avgDewPoint = _dailyCalculator->getAvgIndoorMeasurementFromDate(d, records, "dewPoint");
        double avgHumidex = _dailyCalculator->getAvgIndoorMeasurementFromDate(d, records, "humidex");
        double avgPressure = _dailyCalculator->getAvgIndoorMeasurementFromDate(d, records, "pressure");
        double avgCO2 = _dailyCalculator->getAvgIndoorMeasurementFromDate(d, records, "co2");
        double avgNoise = _dailyCalculator->getAvgIndoorMeasurementFromDate(d, records, "noise");

        IntDailyRecord record = IntDailyRecord(
            d,
            maxTemperature.first.toDouble(),
            minTemperature.first.toDouble(),
            avgTemperature,
            maxHumidity.first.toInt(),
            minHumidity.first.toInt(),
            avgHumidity,
            maxDewPoint.first.toDouble(),
            minDewPoint.first.toDouble(),
            avgDewPoint,
            maxHumidex.first.toDouble(),
            minHumidex.first.toDouble(),
            avgHumidex,
            maxPressure.first.toDouble(),
            minPressure.first.toDouble(),
            avgPressure,
            maxCO2.first.toInt(),
            minCO2.first.toInt(),
            avgCO2,
            maxNoise.first.toInt(),
            minNoise.first.toInt(),
            avgNoise,
            maxTemperature.second,
            minTemperature.second,
            maxHumidity.second,
            minHumidity.second,
            maxDewPoint.second,
            minDewPoint.second,
            maxHumidex.second,
            minHumidex.second,
            maxPressure.second,
            minPressure.second
            );
        dbHandlerProd->postIndoorDailyRecord(record, "IndoorDailyRecords");
        dbHandlerCopy->postIndoorDailyRecord(record, "IndoorDailyRecords");
    }
}

void OldDataUploader::setAccessToken(QString accessToken) {
    _accessToken = accessToken;
}

void OldDataUploader::logExtDailyRecord(ExtDailyRecord record) {
    ExtDailyRecord *recordCopy = new ExtDailyRecord(record);
    QString tableName = "OutdoorDailyRecords";
    QDate date = record.date();
    if (extendedRecordsMap.contains(date)) {
        extendedRecordsMap[date]->setExtDailyRecord(recordCopy);
        dbHandlerCopy->postOutdoorDailyRecord(extendedRecordsMap[date]->wrap(), tableName);
    }
    else {
        extendedRecordsMap.insert(date, new ExtendedExtDailyRecord());
        extendedRecordsMap[date]->setExtDailyRecord(recordCopy);
    }
}

void OldDataUploader::logIntDailyRecord(IntDailyRecord record) {
    QString tableName = "IndoorDailyRecords";
    dbHandlerCopy->postIndoorDailyRecord(record, tableName);
}

void OldDataUploader::log3hRecords(QMap<QDate, std::tuple<double, double>> records) {
    QString tableName = "OutdoorDailyRecords";
    for (QDate date : records.keys()) {
        if (extendedRecordsMap.contains(date)) {
            extendedRecordsMap[date]->setMinTemperature(std::get<0>(records[date]));
            extendedRecordsMap[date]->setMaxTemperature(std::get<1>(records[date]));
            dbHandlerCopy->postOutdoorDailyRecord(extendedRecordsMap[date]->wrap(), tableName);
        }
        else {
            extendedRecordsMap.insert(date, new ExtendedExtDailyRecord());
            extendedRecordsMap[date]->setMinTemperature(std::get<0>(records[date]));
            extendedRecordsMap[date]->setMaxTemperature(std::get<1>(records[date]));
        }
    }
}

void OldDataUploader::logOutdoorTimestampRecords(QList<ExtTimestampRecord> records) {
    for (ExtTimestampRecord record : records) {
        dbHandlerCopy->postOutdoorTimestampRecord(record, "LastOutdoorTimestampRecords");
    }
    emit outdoorTimestampRecordsLogged();
}

void OldDataUploader::logIndoorTimestampRecords(QList<IntTimestampRecord> records) {
    for (IntTimestampRecord record : records) {
        dbHandlerCopy->postIndoorTimestampRecord(record, "LastIndoorTimestampRecords");
    }
    emit indoorTimestampRecordsLogged();
}
