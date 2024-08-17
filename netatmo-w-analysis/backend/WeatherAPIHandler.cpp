#include "WeatherAPIHandler.h"
#include <QUrlQuery>

WeatherAPIHandler::WeatherAPIHandler(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(retrieveWeather(QNetworkReply*)));
}

void WeatherAPIHandler::postWeatherRequest() {
    extern const QString longitude;
    extern const QString latitude;
    extern const QString openWeatherMapAPIKey;

    QUrl url("https://api.openweathermap.org/data/2.5/forecast");
    QUrlQuery params;

    params.addQueryItem("lat", latitude);
    params.addQueryItem("lon", longitude);
    params.addQueryItem("appid", openWeatherMapAPIKey);

    url.setQuery(params);

    QNetworkRequest request(url);
    manager->get(request);
}

void WeatherAPIHandler::retrieveWeather(QNetworkReply *reply) {
    QByteArray bytes = reply->readAll();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with weather API" << bytes;
    }
    else if (bytes.size() >= 1) {
        WeatherPrediction *weatherPrediction = new WeatherPrediction(bytes);
        emit predictionDataRetrieved(*weatherPrediction);
    }
}
