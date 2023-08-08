#ifndef PREDICTIONITEM_H
#define PREDICTIONITEM_H

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

class PredictionItem
{
public:
    PredictionItem();
    PredictionItem(QJsonObject jsonObject);

    QDateTime dateTime();
    double temperature();
    double feltTemperature();
    double minTemperature();
    double maxTemperature();
    double pressure();

    int humidity();

    int weatherId();
    QString weatherMain();
    QString weatherDescription();
    QString weatherIcon();

    int clouds();

    double windSpeed();
    double windGust();
    int windAngle();

    QString toString();

private:
    QDateTime _dateTime;
    double _temperature;
    double _feltTemperature;
    double _minTemperature;
    double _maxTemperature;
    double _pressure;

    int _humidity;

    int _weatherId;
    QString _weatherMain;
    QString _weatherDescription;
    QString _weatherIcon;

    int _clouds;

    double _windSpeed;
    double _windGust;
    int _windAngle;
};

#endif // PREDICTIONITEM_H
