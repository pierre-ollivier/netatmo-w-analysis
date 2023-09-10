#include "PredictionItem.h"
#include <QJsonArray>
#include "../netatmo-w-analysis/types/TimestampRecord.h"

PredictionItem::PredictionItem()
{

}

PredictionItem::PredictionItem(QJsonObject jsonObject) {
    _dateTime = QDateTime::fromSecsSinceEpoch(jsonObject["dt"].toInt());

    QJsonObject mainObject = jsonObject["main"].toObject();
    _temperature = mainObject["temp"].toDouble();
    _feltTemperature = mainObject["feels_like"].toDouble();
    _minTemperature = mainObject["temp_min"].toDouble();
    _maxTemperature = mainObject["temp_max"].toDouble();
    _pressure = mainObject["pressure"].toDouble();
    _humidity = mainObject["humidity"].toInt();

    QJsonArray weatherArray = jsonObject["weather"].toArray();
    QJsonObject weatherObject = weatherArray[0].toObject();
    _weatherId = weatherObject["id"].toInt();
    _weatherMain = weatherObject["main"].toString();
    _weatherDescription = weatherObject["description"].toString();
    _weatherIcon = weatherObject["icon"].toString();

    _clouds = jsonObject["clouds"].toObject()["all"].toInt();

    QJsonObject windObject = jsonObject["wind"].toObject();
    _windSpeed = windObject["speed"].toDouble();
    _windGust = windObject["gust"].toDouble();
    _windAngle = windObject["deg"].toInt();
}

QDateTime PredictionItem::dateTime() {return _dateTime;}

double PredictionItem::temperature() {return _temperature;}
double PredictionItem::feltTemperature() {return _feltTemperature;}
double PredictionItem::minTemperature() {return _minTemperature;}
double PredictionItem::maxTemperature() {return _maxTemperature;}
double PredictionItem::pressure() {return _pressure;}
int PredictionItem::humidity() {return _humidity;}

int PredictionItem::weatherId() {return _weatherId;}
QString PredictionItem::weatherMain() {return _weatherMain;}
QString PredictionItem::weatherDescription() {return _weatherDescription;}
QString PredictionItem::weatherIcon() {return _weatherIcon;}

int PredictionItem::clouds() {return _clouds;}

double PredictionItem::windSpeed() {return _windSpeed;}
double PredictionItem::windGust() {return _windGust;}
int PredictionItem::windAngle() {return _windAngle;}

QString PredictionItem::toString() {
    TimestampRecord record(0, _temperature - 273.15, _humidity);
    return "Température : " + QString::number(_temperature - 273.15, 'f', 0) + " °C"
            + ", ressentie : " + QString::number(record.humidex(), 'f', 0) + "\n"
            + "Point de rosée : " + QString::number(record.dewPoint(), 'f', 0) + " °C" + "\n"
            + "Température min. : " + QString::number(_minTemperature - 273.15, 'f', 0) + " °C"
            + ", max. : " + QString::number(_maxTemperature - 273.15, 'f', 0) + " °C" + "\n"
            + "Pression : " + QString::number(_pressure) + " hPa"
            + ", humidité relative : " + QString::number(_humidity) + " %" + "\n"
            + "Vitesse du vent : " + QString::number(_windSpeed * 3.6, 'f', 0) + " km/h" + "\n"
            + "Direction : " + QString::number(_windAngle) + "°"
            + ", rafales : " + QString::number(_windGust * 3.6, 'f', 0) + " km/h" + "\n"
            + "Couverture nuageuse : " + QString::number(_clouds) + " %";
}
