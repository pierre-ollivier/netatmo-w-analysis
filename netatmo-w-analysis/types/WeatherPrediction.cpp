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
    return maxTemperature(QDate::currentDate().daysTo(date));
}

double WeatherPrediction::maxTemperature(int daysFromNow) {
    QDateTime currentDateTimeUTC = QDateTime::currentDateTimeUtc();
    int mod = currentDateTimeUTC.time().hour() / 3;

    // Add 2 items to take the maximum temperature from 6 UTC
    return maxTemperatureBetweenIndexes(2 - mod + 8 * daysFromNow, 7 + 2 - mod + 8 * daysFromNow);
}

double WeatherPrediction::minTemperature(QDate date) {
    return minTemperature(QDate::currentDate().daysTo(date));
}

double WeatherPrediction::minTemperature(int daysFromNow) {
    QDateTime currentDateTimeUTC = QDateTime::currentDateTimeUtc();
    int mod = currentDateTimeUTC.time().hour() / 3;

    // Remove 2 items to take the minimum temperature from 18 UTC
    return minTemperatureBetweenIndexes(-2 - mod + 8 * daysFromNow, 7 - 2 - mod + 8 * daysFromNow);
}

double WeatherPrediction::maxTemperatureBetweenIndexes(int firstIndex, int lastIndex) {
    if (firstIndex < 0) firstIndex = 0;
    if (lastIndex >= NUMBER_OF_PREDICTION_ITEMS)  lastIndex = NUMBER_OF_PREDICTION_ITEMS - 1;
    double maxTemperature = -qInf();

    for (int i = firstIndex; i <= lastIndex; i++) {
        PredictionItem item = _predictionItems.at(i);
        double partialMaxTemperature = item.maxTemperature() - 273.15;  // convert to °C
        if (partialMaxTemperature > maxTemperature) maxTemperature = partialMaxTemperature;
    }
    return maxTemperature;
}

double WeatherPrediction::minTemperatureBetweenIndexes(int firstIndex, int lastIndex) {
    if (firstIndex < 0) firstIndex = 0;
    if (lastIndex >= NUMBER_OF_PREDICTION_ITEMS)  lastIndex = NUMBER_OF_PREDICTION_ITEMS - 1;
    double minTemperature = qInf();

    for (int i = firstIndex; i <= lastIndex; i++) {
        PredictionItem item = _predictionItems.at(i);
        double partialMinTemperature = item.minTemperature() - 273.15;  // convert to °C
        if (partialMinTemperature < minTemperature) minTemperature = partialMinTemperature;
    }
    return minTemperature;
}
