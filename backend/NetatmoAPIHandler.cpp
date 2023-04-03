#include "NetatmoAPIHandler.h"

NetatmoAPIHandler::NetatmoAPIHandler()
{
    tokensManager = new QNetworkAccessManager();
    currentConditionsManager = new QNetworkAccessManager();

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
}

void NetatmoAPIHandler::postCurrentConditionsRequest() {

    extern const QString deviceId;

    QUrl url("https://api.netatmo.com/api/getstationsdata?");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem("access_token", accessToken.toUtf8());
    params.addQueryItem("device_id", deviceId);
    params.addQueryItem("get_favorites", "false");
    currentConditionsManager->post(request, params.query().toUtf8());
//    timerActualisation->start(delaiActualisation);

}
