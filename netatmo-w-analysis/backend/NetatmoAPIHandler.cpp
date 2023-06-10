#include "NetatmoAPIHandler.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

NetatmoAPIHandler::NetatmoAPIHandler(APIMonitor *monitor, int timeBetweenRequests)
{
    tokensManager = new QNetworkAccessManager();
    currentConditionsManager = new QNetworkAccessManager();
    dailyFullOutdoorRequestManager = new QNetworkAccessManager();
    dailyFullIndoorRequestManager = new QNetworkAccessManager();
    outdoorChartRequestManager = new QNetworkAccessManager();
    indoorChartRequestManager = new QNetworkAccessManager();

    apiMonitor = monitor;

    connect(tokensManager, SIGNAL(finished(QNetworkReply *)), SLOT(retrieveTokens(QNetworkReply *)));
    connect(currentConditionsManager, SIGNAL(finished(QNetworkReply *)), SLOT(retrieveCurrentConditions(QNetworkReply *)));
    connect(dailyFullOutdoorRequestManager, SIGNAL(finished(QNetworkReply *)), SLOT(retrieveFullDailyOutdoorConditions(QNetworkReply *)));
    connect(dailyFullIndoorRequestManager, SIGNAL(finished(QNetworkReply *)), SLOT(retrieveFullDailyIndoorConditions(QNetworkReply *)));
    connect(outdoorChartRequestManager, SIGNAL(finished(QNetworkReply *)), SLOT(retrieveOutdoorChartRequest(QNetworkReply *)));
    connect(indoorChartRequestManager, SIGNAL(finished(QNetworkReply *)), SLOT(retrieveIndoorChartRequest(QNetworkReply *)));

    currentConditionsTimer = new QTimer();
    if (timeBetweenRequests > 0) {
        currentConditionsTimer->start(timeBetweenRequests);
    }
    connect(currentConditionsTimer, SIGNAL(timeout()), this, SLOT(postCurrentConditionsRequest()));

}

NetatmoAPIHandler::NetatmoAPIHandler(NetatmoAPIHandler &other) :
    NetatmoAPIHandler(other.getAPIMonitor(), other.getTimeBetweenRequests()) {
    accessToken = other.getAccessToken();
}

APIMonitor* NetatmoAPIHandler::getAPIMonitor() {
    return apiMonitor;
}

int NetatmoAPIHandler::getTimeBetweenRequests() {
    return currentConditionsTimer->interval();
}

QString NetatmoAPIHandler::getAccessToken() {
    return accessToken;
}

void NetatmoAPIHandler::setAccessToken(QString newAccessToken) {
    accessToken = newAccessToken;
}

void NetatmoAPIHandler::postTokensRequest() {
    extern const QString username;
    extern const QString password;
    extern const QString clientId;
    extern const QString clientSecret;

    QUrl url("https://api.netatmo.com/oauth2/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("grant_type", "password");
    params.addQueryItem("client_id", clientId);
    params.addQueryItem("client_secret", clientSecret);
    params.addQueryItem("username", username);
    params.addQueryItem("password", password);
    params.addQueryItem("scope", "read_station");
    tokensManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();
}

void NetatmoAPIHandler::postCurrentConditionsRequest(QString accessToken) {
    if (accessToken == "") qDebug() << "Warning: undefined access token in NetatmoAPIHandler";
    extern const QString mainDeviceId;

    QUrl url("https://api.netatmo.com/api/getstationsdata?");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem("access_token", accessToken.toUtf8());
    params.addQueryItem("device_id", mainDeviceId);
    params.addQueryItem("get_favorites", "false");
    currentConditionsManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();
}

void NetatmoAPIHandler::postCurrentConditionsRequest() {
    if (accessToken == "") qDebug() << "Warning: undefined access token in NetatmoAPIHandler (postCurCondRqst)";
    extern const QString mainDeviceId;

    QUrl url("https://api.netatmo.com/api/getstationsdata?");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem("access_token", accessToken.toUtf8());
    params.addQueryItem("device_id", mainDeviceId);
    params.addQueryItem("get_favorites", "false");
    currentConditionsManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();
}

void NetatmoAPIHandler::postFullIndoorDailyRequest(int date_begin, int date_end, QString scale, QString accessToken) {
    if (accessToken == "") qDebug() << "Warning: undefined access token in NetatmoAPIHandler";
    extern const QString mainDeviceId;
    extern const QString indoorModuleId;

    QUrl url("https://api.netatmo.com/api/getmeasure?");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem("access_token", accessToken.toUtf8());
    params.addQueryItem("device_id", mainDeviceId);
    params.addQueryItem("module_id", indoorModuleId);
    params.addQueryItem("scale", scale);
    params.addQueryItem("type", "max_temp,min_temp,temperature,date_max_temp,date_min_temp,"
                                "max_hum,min_hum,humidity,date_max_hum,date_min_hum,"
                                "max_pressure,min_pressure,pressure,date_max_pressure,date_min_pressure,"
                                "max_co2,min_co2,co2,date_max_co2,date_min_co2,"
                                "max_noise,min_noise,noise,date_max_noise,date_min_noise");
    params.addQueryItem("date_begin", QString::number(date_begin));
    params.addQueryItem("date_end", QString::number(date_end));
    params.addQueryItem("optimize", "false");
    params.addQueryItem("real_time", "true");
    dailyFullIndoorRequestManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();
}

void NetatmoAPIHandler::postFullOutdoorDailyRequest(int date_begin, int date_end, QString scale, QString accessToken) {
    if (accessToken == "") qDebug() << "Warning: undefined access token in NetatmoAPIHandler";
    extern const QString mainDeviceId;
    extern const QString outdoorModuleId;

    QUrl url("https://api.netatmo.com/api/getmeasure?");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem("access_token", accessToken.toUtf8());
    params.addQueryItem("device_id", mainDeviceId);
    params.addQueryItem("module_id", outdoorModuleId);
    params.addQueryItem("scale", scale);
    params.addQueryItem("type", "max_temp,min_temp,temperature,date_max_temp,date_min_temp,max_hum,min_hum,humidity,date_max_hum,date_min_hum");
    params.addQueryItem("date_begin", QString::number(date_begin));
    params.addQueryItem("date_end", QString::number(date_end));
    params.addQueryItem("optimize", "false");
    params.addQueryItem("real_time", "true");
    dailyFullOutdoorRequestManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();
}

void NetatmoAPIHandler::postOutdoorChartRequest(int date_begin, QString scale, QString accessToken) {
    if (accessToken == "") qDebug() << "Warning: undefined access token in NetatmoAPIHandler";
    extern const QString mainDeviceId;
    extern const QString outdoorModuleId;

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
}

void NetatmoAPIHandler::postIndoorChartRequest(int date_begin, QString scale, QString accessToken) {
    if (accessToken == "") qDebug() << "Warning: undefined access token in NetatmoAPIHandler";
    extern const QString mainDeviceId;
    extern const QString outdoorModuleId;

    QUrl url("https://api.netatmo.com/api/getmeasure?");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem("access_token", accessToken.toUtf8());
    params.addQueryItem("device_id", mainDeviceId);
    params.addQueryItem("module_id", mainDeviceId);
    params.addQueryItem("scale", scale);
    params.addQueryItem("type", "temperature,humidity");
    params.addQueryItem("date_begin", QString::number(date_begin));
    params.addQueryItem("optimize", "false");
    params.addQueryItem("real_time", "true");
    indoorChartRequestManager->post(request, params.query().toUtf8());
}

void NetatmoAPIHandler::retrieveTokens(QNetworkReply *reply) {
    QByteArray bytes = reply->readAll();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with tokens" << bytes;
    }
    else if (bytes.size() >= 1) {
        QJsonDocument js = QJsonDocument::fromJson(bytes);

        accessToken = js["access_token"].toString();
        refreshToken = js["refresh_token"].toString();

        emit accessTokenChanged(accessToken);
        emit refreshTokenChanged(refreshToken);
    }
    else {
        qDebug() << "ERROR with network"
                 << "Impossible de recevoir le token. Vérifier la connexion et redémarrer le programme.";
    }
}

void NetatmoAPIHandler::retrieveCurrentConditions(QNetworkReply *reply) {
    QByteArray bytes = reply->readAll();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with current conditions" << bytes;
    }
    else if (bytes.size() >= 1) {
        QJsonDocument js = QJsonDocument::fromJson(bytes);

        extCurrentTemperature = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["Temperature"].toDouble();
        extCurrentMinTemperature = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["min_temp"].toDouble();
        extCurrentMaxTemperature = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["max_temp"].toDouble();
        extCurrentHumidity = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["Humidity"].toInt();
        extCurrentMinTemperatureTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["date_min_temp"].toInt();
        extCurrentMaxTemperatureTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["date_max_temp"].toInt();

        if (extCurrentUTCTime != js["body"]["devices"][0]["modules"][0]["dashboard_data"]["time_utc"].toInt()) {
            extCurrentUTCTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["time_utc"].toInt();
            emit extUTCTimeChanged(extCurrentUTCTime);
        }

        intCurrentTemperature = js["body"]["devices"][0]["dashboard_data"]["Temperature"].toDouble();
        intCurrentMinTemperature = js["body"]["devices"][0]["dashboard_data"]["min_temp"].toDouble();
        intCurrentMaxTemperature = js["body"]["devices"][0]["dashboard_data"]["max_temp"].toDouble();
        intCurrentHumidity = js["body"]["devices"][0]["dashboard_data"]["Humidity"].toInt();
        intCurrentMinTemperatureTime = js["body"]["devices"][0]["dashboard_data"]["date_min_temp"].toInt();
        intCurrentMaxTemperatureTime = js["body"]["devices"][0]["dashboard_data"]["date_max_temp"].toInt();
        intCurrentPressure = js["body"]["devices"][0]["dashboard_data"]["Pressure"].toDouble();
        intCurrentCO2 = js["body"]["devices"][0]["dashboard_data"]["CO2"].toInt();
        intCurrentNoise = js["body"]["devices"][0]["dashboard_data"]["Noise"].toInt();

        if (intCurrentUTCTime != js["body"]["devices"][0]["dashboard_data"]["time_utc"].toInt()) {
            intCurrentUTCTime = js["body"]["devices"][0]["dashboard_data"]["time_utc"].toInt();
            emit intUTCTimeChanged(intCurrentUTCTime);
        }

        emit extTemperatureChanged(extCurrentTemperature);
        emit extMinTemperatureChanged(extCurrentMinTemperature);
        emit extMaxTemperatureChanged(extCurrentMaxTemperature);
        emit extHumidityChanged(extCurrentHumidity);
        emit extMinTemperatureTimeChanged(extCurrentMinTemperatureTime);
        emit extMaxTemperatureTimeChanged(extCurrentMaxTemperatureTime);

        emit intTemperatureChanged(intCurrentTemperature);
        emit intMinTemperatureChanged(intCurrentMinTemperature);
        emit intMaxTemperatureChanged(intCurrentMaxTemperature);
        emit intHumidityChanged(intCurrentHumidity);
        emit intMinTemperatureTimeChanged(intCurrentMinTemperatureTime);
        emit intMaxTemperatureTimeChanged(intCurrentMaxTemperatureTime);

        emit currentTimeChanged(QDateTime::currentDateTime());
    }
    else {
        qDebug() << "ERROR with network"
                 << "Impossible de recevoir le token. Vérifier la connexion et redémarrer le programme.";
    }
}

void NetatmoAPIHandler::retrieveFullDailyOutdoorConditions(QNetworkReply *reply) {
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with full daily outdoor conditions" << bytes;
    }

    else if (bytes.size() >= 1) {
        QJsonDocument js = QJsonDocument::fromJson(bytes);
        foreach (const QString &key, tb.keys()) {
            QJsonValue value = tb.value(key);
            long long timestamp = key.toLongLong();
            emit extDailyRecordRetrieved(
                        ExtDailyRecord(
                            QDateTime::fromSecsSinceEpoch(timestamp).date(),
                            value[0].toDouble(),
                            value[1].toDouble(),
                            value[2].toDouble(),
                            value[5].toDouble(),
                            value[6].toDouble(),
                            value[7].toDouble(),
                            value[3].toInt(),
                            value[4].toInt(),
                            value[8].toInt(),
                            value[9].toInt()
                        )
            );
        }
    }
}

void NetatmoAPIHandler::retrieveFullDailyIndoorConditions(QNetworkReply *reply) {
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with full daily indoor conditions" << bytes;
    }

    else if (bytes.size() >= 1) {
        QJsonDocument js = QJsonDocument::fromJson(bytes);
        foreach (const QString &key, tb.keys()) {
            QJsonValue value = tb.value(key);
            long long timestamp = key.toLongLong();
            emit intDailyRecordRetrieved(
                        IntDailyRecord(
                            QDateTime::fromSecsSinceEpoch(timestamp).date(),
                            value[0].toDouble(),
                            value[1].toDouble(),
                            value[2].toDouble(),
                            value[5].toDouble(),
                            value[6].toDouble(),
                            value[7].toDouble(),
                            value[10].toDouble(),
                            value[11].toDouble(),
                            value[12].toDouble(),
                            value[15].toDouble(),
                            value[16].toDouble(),
                            value[17].toDouble(),
                            value[20].toDouble(),
                            value[21].toDouble(),
                            value[22].toDouble(),
                            value[3].toInt(),
                            value[4].toInt(),
                            value[8].toInt(),
                            value[9].toInt(),
                            value[13].toInt(),
                            value[14].toInt()
                        )
            );
        }
    }
}

void NetatmoAPIHandler::retrieveOutdoorChartRequest(QNetworkReply *reply) {
    QList<QPointF> temperatureList = QList<QPointF>();
    QList<QPointF> humidityList = QList<QPointF>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());
            temperatureList.append(QPointF(1000 * key.toLongLong(), temperature));
            humidityList.append(QPointF(1000 * key.toLongLong(), humidity));
        }
        emit outdoorTemperatureListRetrieved(temperatureList);
        emit outdoorHumidityListRetrieved(humidityList);
    }
}

void NetatmoAPIHandler::retrieveIndoorChartRequest(QNetworkReply *reply) {
    QList<QPointF> temperatureList = QList<QPointF>();
    QList<QPointF> humidityList = QList<QPointF>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        foreach (const QString &key, tb.keys()) {
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());
            temperatureList.append(QPointF(1000 * key.toLongLong(), temperature));
            humidityList.append(QPointF(1000 * key.toLongLong(), humidity));
        }
        emit indoorTemperatureListRetrieved(temperatureList);
        emit indoorHumidityListRetrieved(humidityList);
    }
}
