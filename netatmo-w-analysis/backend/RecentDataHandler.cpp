#include "RecentDataHandler.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

extern const QString PATH_TO_COPY_DATABASE;

RecentDataHandler::RecentDataHandler(APIMonitor *monitor)
{
    apiMonitor = monitor;

    outdoorChartRequestManager = new QNetworkAccessManager(this);
    indoorChartRequestManager = new QNetworkAccessManager(this);
    longOutdoorLastRequestManager = new QNetworkAccessManager(this);
    longOutdoorChartRequestManager = new QNetworkAccessManager(this);
    longIndoorLastRequestManager = new QNetworkAccessManager(this);
    longIndoorChartRequestManager = new QNetworkAccessManager(this);

    dbHandlerLastRecords = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);

    connect(outdoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveOutdoorChartRequest(QNetworkReply*)));
    connect(indoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveIndoorChartRequest(QNetworkReply*)));
    connect(longOutdoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongOutdoorChartRequest(QNetworkReply*)));
    connect(longOutdoorLastRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongOutdoorLastRequest(QNetworkReply*)));
    connect(longIndoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongIndoorChartRequest(QNetworkReply*)));
    connect(longIndoorLastRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongIndoorLastRequest(QNetworkReply*)));
}

RecentDataHandler::RecentDataHandler(QObject *parent, APIMonitor *monitor) : QObject(parent)
{
    apiMonitor = monitor;

    outdoorChartRequestManager = new QNetworkAccessManager(this);
    indoorChartRequestManager = new QNetworkAccessManager(this);
    longOutdoorLastRequestManager = new QNetworkAccessManager(this);
    longOutdoorChartRequestManager = new QNetworkAccessManager(this);
    longIndoorLastRequestManager = new QNetworkAccessManager(this);
    longIndoorChartRequestManager = new QNetworkAccessManager(this);

    dbHandlerLastRecords = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);

    connect(outdoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveOutdoorChartRequest(QNetworkReply*)));
    connect(indoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveIndoorChartRequest(QNetworkReply*)));
    connect(longOutdoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongOutdoorChartRequest(QNetworkReply*)));
    connect(longOutdoorLastRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongOutdoorLastRequest(QNetworkReply*)));
    connect(longIndoorChartRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongIndoorChartRequest(QNetworkReply*)));
    connect(longIndoorLastRequestManager, SIGNAL(finished(QNetworkReply*)), SLOT(retrieveLongIndoorLastRequest(QNetworkReply*)));
}

void RecentDataHandler::postRequests(int date_begin, QString scale, QString accessToken) {
    if (accessToken == "") qDebug() << "Warning: undefined access token in RecentDataHandler";
    extern const QString mainDeviceId;
    extern const QString outdoorModuleId;

    if (scale == "max") {
        QUrl url("https://api.netatmo.com/api/getmeasure?");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QUrlQuery params;
        params.addQueryItem("access_token", accessToken.toUtf8());
        params.addQueryItem("device_id", mainDeviceId);
        params.addQueryItem("module_id", outdoorModuleId);
        params.addQueryItem("scale", scale);
        params.addQueryItem("type", "temperature,humidity");
        params.addQueryItem("date_begin", QString::number(date_begin));
        params.addQueryItem("optimize", "false");
        params.addQueryItem("real_time", "true");
        outdoorChartRequestManager->post(request, params.query().toUtf8());
        apiMonitor->addTimestamp();

        url = QUrl("https://api.netatmo.com/api/getmeasure?");
        request = QNetworkRequest(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        params = QUrlQuery();
        params.addQueryItem("access_token", accessToken.toUtf8());
        params.addQueryItem("device_id", mainDeviceId);
        params.addQueryItem("module_id", mainDeviceId);
        params.addQueryItem("scale", scale);
        params.addQueryItem("type", "temperature,humidity,pressure,co2,noise");
        params.addQueryItem("date_begin", QString::number(date_begin));
        params.addQueryItem("optimize", "false");
        params.addQueryItem("real_time", "true");
        indoorChartRequestManager->post(request, params.query().toUtf8());
        apiMonitor->addTimestamp();
    }

    else {
        QUrl url("https://api.netatmo.com/api/getmeasure?");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QUrlQuery params;
        params.addQueryItem("access_token", accessToken.toUtf8());
        params.addQueryItem("device_id", mainDeviceId);
        params.addQueryItem("module_id", outdoorModuleId);
        params.addQueryItem("scale", scale);
        params.addQueryItem("type", "temperature,humidity");
        params.addQueryItem("date_begin", QString::number(date_begin));
        params.addQueryItem("optimize", "false");
        params.addQueryItem("real_time", "true");
        longOutdoorChartRequestManager->post(request, params.query().toUtf8());
        apiMonitor->addTimestamp();

        url = QUrl("https://api.netatmo.com/api/getmeasure?");
        request = QNetworkRequest(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        params = QUrlQuery();
        params.addQueryItem("access_token", accessToken.toUtf8());
        params.addQueryItem("device_id", mainDeviceId);
        params.addQueryItem("module_id", outdoorModuleId);
        params.addQueryItem("scale", "max");
        params.addQueryItem("type", "temperature,humidity");
        params.addQueryItem("date_begin", QString::number(QDateTime::currentSecsSinceEpoch() - 24 * 3600));
        params.addQueryItem("optimize", "false");
        params.addQueryItem("real_time", "true");
        longOutdoorLastRequestManager->post(request, params.query().toUtf8());
        apiMonitor->addTimestamp();

        url = QUrl("https://api.netatmo.com/api/getmeasure?");
        request = QNetworkRequest(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        params = QUrlQuery();
        params.addQueryItem("access_token", accessToken.toUtf8());
        params.addQueryItem("device_id", mainDeviceId);
        params.addQueryItem("module_id", mainDeviceId);
        params.addQueryItem("scale", scale);
        params.addQueryItem("type", "temperature,humidity,pressure,co2,noise");
        params.addQueryItem("date_begin", QString::number(date_begin));
        params.addQueryItem("optimize", "false");
        params.addQueryItem("real_time", "true");
        longIndoorChartRequestManager->post(request, params.query().toUtf8());
        apiMonitor->addTimestamp();

        url = QUrl("https://api.netatmo.com/api/getmeasure?");
        request = QNetworkRequest(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        params = QUrlQuery();
        params.addQueryItem("access_token", accessToken.toUtf8());
        params.addQueryItem("device_id", mainDeviceId);
        params.addQueryItem("module_id", mainDeviceId);
        params.addQueryItem("scale", "max");
        params.addQueryItem("type", "temperature,humidity,pressure,co2,noise");
        params.addQueryItem("date_begin", QString::number(QDateTime::currentSecsSinceEpoch() - 24 * 3600));
        params.addQueryItem("optimize", "false");
        params.addQueryItem("real_time", "true");
        longIndoorLastRequestManager->post(request, params.query().toUtf8());
        apiMonitor->addTimestamp();
    }
}


void RecentDataHandler::retrieveOutdoorChartRequest(QNetworkReply *reply) {
    QList<ExtTimestampRecord> recordsList = QList<ExtTimestampRecord>();
    QList<ExtTimestampRecord> lastRecordsList = QList<ExtTimestampRecord>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            long long timestamp = key.toLongLong();
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());

            recordsList.append(ExtTimestampRecord(timestamp, temperature, humidity));
            if (timestamp > dbHandlerLastRecords->getLatestTimestampFromDatabaseInS("LastOutdoorTimestampRecords")) {
                lastRecordsList.append(ExtTimestampRecord(timestamp, temperature, humidity));
            }
        }
        emit outdoorRecordListRetrieved(recordsList);
        emit recentOutdoorRecordListRetrieved(lastRecordsList);
    }
}


void RecentDataHandler::retrieveIndoorChartRequest(QNetworkReply *reply) {
    QList<IntTimestampRecord> recordsList = QList<IntTimestampRecord>();
    QList<IntTimestampRecord> lastRecordsList = QList<IntTimestampRecord>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            long long timestamp = key.toLongLong();
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());
            double pressure = value[2].toDouble();
            int co2 = value[3].toDouble();
            int noise = value[4].toDouble();

            recordsList.append(IntTimestampRecord(timestamp, temperature, humidity, pressure, co2, noise));
            if (timestamp > dbHandlerLastRecords->getLatestTimestampFromDatabaseInS("LastIndoorTimestampRecords")) {
                lastRecordsList.append(IntTimestampRecord(timestamp, temperature, humidity, pressure, co2, noise));
            }
        }
        emit indoorRecordListRetrieved(recordsList);
        emit recentIndoorRecordListRetrieved(lastRecordsList);
    }
}


void RecentDataHandler::retrieveLongOutdoorChartRequest(QNetworkReply *reply) {
    QList<ExtTimestampRecord> recordsList = QList<ExtTimestampRecord>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            long long timestamp = key.toLongLong();
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());

            recordsList.append(ExtTimestampRecord(timestamp, temperature, humidity));
        }
        emit outdoorRecordListRetrieved(recordsList);
    }
}


void RecentDataHandler::retrieveLongOutdoorLastRequest(QNetworkReply *reply) {
    QList<ExtTimestampRecord> lastRecordsList = QList<ExtTimestampRecord>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            long long timestamp = key.toLongLong();
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());

            if (timestamp > dbHandlerLastRecords->getLatestTimestampFromDatabaseInS("LastOutdoorTimestampRecords")) {
                lastRecordsList.append(ExtTimestampRecord(timestamp, temperature, humidity));
            }
        }
        emit recentOutdoorRecordListRetrieved(lastRecordsList);
    }
}


void RecentDataHandler::retrieveLongIndoorChartRequest(QNetworkReply *reply) {
    QList<IntTimestampRecord> recordsList = QList<IntTimestampRecord>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            long long timestamp = key.toLongLong();
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());
            double pressure = value[2].toDouble();
            int co2 = value[3].toDouble();
            int noise = value[4].toDouble();
            recordsList.append(IntTimestampRecord(timestamp, temperature, humidity,pressure, co2, noise));
        }
        emit indoorRecordListRetrieved(recordsList);
    }
}


void RecentDataHandler::retrieveLongIndoorLastRequest(QNetworkReply *reply) {
    QList<IntTimestampRecord> lastRecordsList = QList<IntTimestampRecord>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            long long timestamp = key.toLongLong();
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());
            double pressure = value[2].toDouble();
            int co2 = value[3].toDouble();
            int noise = value[4].toDouble();

            if (timestamp > dbHandlerLastRecords->getLatestTimestampFromDatabaseInS("LastIndoorTimestampRecords")) {
                lastRecordsList.append(IntTimestampRecord(timestamp, temperature, humidity, pressure, co2, noise));
            }
        }
        emit recentIndoorRecordListRetrieved(lastRecordsList);
    }
}
