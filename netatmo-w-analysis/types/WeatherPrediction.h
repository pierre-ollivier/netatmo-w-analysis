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
    PredictionItem itemAt(int i);

    double maxTemperature(QDate date);
    double maxTemperature(int daysFromNow);

    double minTemperature(QDate date);
    double minTemperature(int daysFromNow);

    QString dayPictogram(int daysFromNow);

private:
    double _longitude = 0;
    double _latitude = 0;

    QDateTime _sunrise = QDateTime();
    QDateTime _sunset = QDateTime();

    QList<PredictionItem> _predictionItems = QList<PredictionItem>();

    double maxTemperatureBetweenIndexes(int firstIndex, int lastIndex);
    double minTemperatureBetweenIndexes(int firstIndex, int lastIndex);
};

#endif // WEATHERPREDICTION_H
