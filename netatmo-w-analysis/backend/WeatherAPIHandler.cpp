#include "WeatherAPIHandler.h"
#include <QUrlQuery>

WeatherAPIHandler::WeatherAPIHandler()
{
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply *)),
            SLOT(retrieveWeather(QNetworkReply *)));
}

void WeatherAPIHandler::postWeatherRequest() {
    extern const QString infoclimatAuth;
    extern const QString infoclimatC;
    QUrl url("https://www.infoclimat.fr/public-api/gfs/json?");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("_ll", "48.894410,2.336951");
    params.addQueryItem("_auth", infoclimatAuth);
    params.addQueryItem("_c", infoclimatC);
    manager->post(request, params.query().toUtf8());
}

void WeatherAPIHandler::retrieveWeather(QNetworkReply *reply) {
    QByteArray bytes = reply->readAll();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with current conditions" << bytes;
    }
    else if (bytes.size() >= 1) {
        qDebug() << bytes;
    }
}
