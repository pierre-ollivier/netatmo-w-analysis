#include "PredictionItem.h"

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
}

QDateTime PredictionItem::dateTime() {return _dateTime;}

double PredictionItem::temperature() {return _temperature;}
double PredictionItem::feltTemperature() {return _feltTemperature;}
double PredictionItem::minTemperature() {return _minTemperature;}
double PredictionItem::maxTemperature() {return _maxTemperature;}
double PredictionItem::pressure() {return _pressure;}

int PredictionItem::humidity() {return _humidity;}
