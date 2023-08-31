#include "WeatherPrediction.h"
#include <QJsonDocument>

const int NUMBER_OF_PREDICTION_ITEMS = 40;

WeatherPrediction::WeatherPrediction()
{

}

WeatherPrediction::WeatherPrediction(QByteArray bytes) {
    QJsonDocument js = QJsonDocument::fromJson(bytes);

    _longitude = js["city"]["coord"]["lon"].toDouble();
    _latitude = js["city"]["coord"]["lat"].toDouble();

    _sunrise = QDateTime::fromSecsSinceEpoch(js["city"]["sunrise"].toInt());
    _sunset = QDateTime::fromSecsSinceEpoch(js["city"]["sunset"].toInt());

    for (int i = 0; i < NUMBER_OF_PREDICTION_ITEMS; i++) {
        _predictionItems.append(PredictionItem(js["list"][i].toObject()));
    }
}

double WeatherPrediction::longitude() {return _longitude;}
double WeatherPrediction::latitude() {return _latitude;}

QDateTime WeatherPrediction::sunrise() {return _sunrise;}
QDateTime WeatherPrediction::sunset() {return _sunset;}

PredictionItem WeatherPrediction::firstItem() {return _predictionItems.at(0);}
PredictionItem WeatherPrediction::itemAt(int i) {return _predictionItems.at(i);}

double WeatherPrediction::maxTemperature(QDate date) {
    return 0;  // TODO
}
