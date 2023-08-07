#ifndef WEATHERPREDICTION_H
#define WEATHERPREDICTION_H

#include <QByteArray>
#include <QDateTime>

class WeatherPrediction
{
public:
    WeatherPrediction();
    WeatherPrediction(QByteArray bytes);

    double latitude();
    double longitude();

    QDateTime sunrise();
    QDateTime sunset();

private:
    double _longitude = 0;
    double _latitude = 0;
    QDateTime _sunrise = QDateTime();
    QDateTime _sunset = QDateTime();
};

#endif // WEATHERPREDICTION_H
