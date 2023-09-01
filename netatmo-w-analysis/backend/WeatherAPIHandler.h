#ifndef WEATHERAPIHANDLER_H
#define WEATHERAPIHANDLER_H

#include <QNetworkReply>
#include <QObject>
#include "../netatmo-w-analysis/types/WeatherPrediction.h"

class WeatherAPIHandler : public QObject
{
    Q_OBJECT
public:
    WeatherAPIHandler();

public slots:
    void postWeatherRequest();
    void retrieveWeather(QNetworkReply *);

signals:
    void predictionDataRetrieved(WeatherPrediction);

private:
    QNetworkAccessManager *manager;
};

#endif // WEATHERAPIHANDLER_H
