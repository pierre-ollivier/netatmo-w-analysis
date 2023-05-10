#include "NetatmoAPIHandler.h"
#include <QJsonDocument>

NetatmoAPIHandler::NetatmoAPIHandler(APIMonitor *monitor, int timeBetweenRequests)
{
    tokensManager = new QNetworkAccessManager();
    currentConditionsManager = new QNetworkAccessManager();
    dailyRequestManager = new QNetworkAccessManager();

    apiMonitor = monitor;

    connect(tokensManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(retrieveTokens(QNetworkReply *)));
    connect(currentConditionsManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(retrieveCurrentConditions(QNetworkReply *)));

    currentConditionsTimer = new QTimer();
    if (timeBetweenRequests > 0) {
        currentConditionsTimer->start(timeBetweenRequests);
    }
    connect(currentConditionsTimer, SIGNAL(timeout()), this, SLOT(postCurrentConditionsRequest()));

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

void NetatmoAPIHandler::postDailyRequest(int date_begin, QString scale, QString accessToken) {

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
    dailyRequestManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();

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
        extCurrentUTCTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["time_utc"].toInt();
        extCurrentMinTemperatureTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["date_min_temp"].toInt();
        extCurrentMaxTemperatureTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["date_max_temp"].toInt();

        intCurrentTemperature = js["body"]["devices"][0]["dashboard_data"]["Temperature"].toDouble();
        intCurrentMinTemperature = js["body"]["devices"][0]["dashboard_data"]["min_temp"].toDouble();
        intCurrentMaxTemperature = js["body"]["devices"][0]["dashboard_data"]["max_temp"].toDouble();
        intCurrentHumidity = js["body"]["devices"][0]["dashboard_data"]["Humidity"].toInt();
        intCurrentUTCTime = js["body"]["devices"][0]["dashboard_data"]["time_utc"].toInt();
        intCurrentMinTemperatureTime = js["body"]["devices"][0]["dashboard_data"]["date_min_temp"].toInt();
        intCurrentMaxTemperatureTime = js["body"]["devices"][0]["dashboard_data"]["date_max_temp"].toInt();
        intCurrentPressure = js["body"]["devices"][0]["dashboard_data"]["Pressure"].toDouble();
        intCurrentCO2 = js["body"]["devices"][0]["dashboard_data"]["CO2"].toInt();
        intCurrentNoise = js["body"]["devices"][0]["dashboard_data"]["Noise"].toInt();

        emit extTemperatureChanged(extCurrentTemperature);
        emit extMinTemperatureChanged(extCurrentMinTemperature);
        emit extMaxTemperatureChanged(extCurrentMaxTemperature);
        emit extHumidityChanged(extCurrentHumidity);
        emit extUTCTimeChanged(extCurrentUTCTime);
        emit extMinTemperatureTimeChanged(extCurrentMinTemperatureTime);
        emit extMaxTemperatureTimeChanged(extCurrentMaxTemperatureTime);

        emit intTemperatureChanged(intCurrentTemperature);
        emit intMinTemperatureChanged(intCurrentMinTemperature);
        emit intMaxTemperatureChanged(intCurrentMaxTemperature);
        emit intHumidityChanged(intCurrentHumidity);
        emit intUTCTimeChanged(intCurrentUTCTime);
        emit intMinTemperatureTimeChanged(intCurrentMinTemperatureTime);
        emit intMaxTemperatureTimeChanged(intCurrentMaxTemperatureTime);

        emit currentTimeChanged(QDateTime::currentDateTime());
    }
    else {
        qDebug() << "ERROR with network"
                 << "Impossible de recevoir le token. Vérifier la connexion et redémarrer le programme.";
    }
}
