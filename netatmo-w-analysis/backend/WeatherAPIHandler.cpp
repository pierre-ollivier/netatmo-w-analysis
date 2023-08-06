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
    extern const QString openWeatherMapAPIKey;
//    QUrl url("https://www.infoclimat.fr/public-api/gfs/json");
    QUrl url("api.openweathermap.org/data/2.5/forecast");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
//    params.addQueryItem("_ll", "48.894410,2.336951");
//    params.addQueryItem("_auth", infoclimatAuth);
//    params.addQueryItem("_c", infoclimatC);
//    params.addQueryItem("%5Fll", "48.894410,2.336951");
//    params.addQueryItem("%5Fauth", infoclimatAuth);
//    params.addQueryItem("%5Fc", infoclimatC);
    params.addQueryItem("lat", "48.894410");
    params.addQueryItem("lon", "2.336951");
    params.addQueryItem("appid", openWeatherMapAPIKey);
    qDebug() << params.query().toUtf8();
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
