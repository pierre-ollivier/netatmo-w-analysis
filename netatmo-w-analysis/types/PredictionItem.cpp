#include "PredictionItem.h"
#include <QJsonArray>

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
