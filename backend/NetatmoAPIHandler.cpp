#include "NetatmoAPIHandler.h"

NetatmoAPIHandler::NetatmoAPIHandler()
{
    tokensManager = new QNetworkAccessManager();

}

void NetatmoAPIHandler::postTokensRequest() {

    QUrl url("https://api.netatmo.com/oauth2/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    extern const QString username;
    extern const QString password;
    extern const QString clientId;
    extern const QString clientSecret;

    QUrlQuery params;
    params.addQueryItem("grant_type", "password");
    params.addQueryItem("client_id", clientId);
    params.addQueryItem("client_secret", clientSecret);
    params.addQueryItem("username", username);
    params.addQueryItem("password", password);
    params.addQueryItem("scope", "read_station");
    tokensManager->post(request, params.query().toUtf8());
}
