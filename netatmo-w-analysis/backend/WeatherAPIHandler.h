#ifndef WEATHERAPIHANDLER_H
#define WEATHERAPIHANDLER_H

#include <QNetworkReply>
#include <QObject>

class WeatherAPIHandler : public QObject
{
    Q_OBJECT
public:
    WeatherAPIHandler();

public slots:
    void postWeatherRequest();
    void retrieveWeather(QNetworkReply *);

private:
    QNetworkAccessManager *manager;
};

#endif // WEATHERAPIHANDLER_H
