#ifndef PREDICTIONITEM_H
#define PREDICTIONITEM_H

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

class PredictionItem
{
public:
    PredictionItem();
//    PredictionItem(QJsonDocument jsonDocument);
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
};

#endif // PREDICTIONITEM_H
