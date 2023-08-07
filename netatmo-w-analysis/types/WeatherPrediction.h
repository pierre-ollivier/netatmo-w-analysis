#ifndef WEATHERPREDICTION_H
#define WEATHERPREDICTION_H

#include <QByteArray>
#include <QDateTime>
#include "../types/PredictionItem.h"

class WeatherPrediction
{
public:
    WeatherPrediction();
    WeatherPrediction(QByteArray bytes);

    double latitude();
    double longitude();

    QDateTime sunrise();
    QDateTime sunset();

    PredictionItem firstItem();

private:
    double _longitude = 0;
    double _latitude = 0;

    QDateTime _sunrise = QDateTime();
    QDateTime _sunset = QDateTime();

    PredictionItem _firstItem = PredictionItem();
};

#endif // WEATHERPREDICTION_H
