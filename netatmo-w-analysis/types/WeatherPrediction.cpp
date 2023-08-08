#include "WeatherPrediction.h"
#include <QJsonDocument>

WeatherPrediction::WeatherPrediction()
{

}

WeatherPrediction::WeatherPrediction(QByteArray bytes) {
    QJsonDocument js = QJsonDocument::fromJson(bytes);

    _longitude = js["city"]["coord"]["lon"].toDouble();
    _latitude = js["city"]["coord"]["lat"].toDouble();

    _sunrise = QDateTime::fromSecsSinceEpoch(js["city"]["sunrise"].toInt());
    _sunset = QDateTime::fromSecsSinceEpoch(js["city"]["sunset"].toInt());

    _firstItem = PredictionItem(js["list"][0].toObject());
}

double WeatherPrediction::longitude() {return _longitude;}
double WeatherPrediction::latitude() {return _latitude;}

QDateTime WeatherPrediction::sunrise() {return _sunrise;}
QDateTime WeatherPrediction::sunset() {return _sunset;}

PredictionItem WeatherPrediction::firstItem() {return _firstItem;}
