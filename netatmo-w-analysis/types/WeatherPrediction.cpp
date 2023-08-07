#include "WeatherPrediction.h"
#include <QJsonDocument>

WeatherPrediction::WeatherPrediction()
{

}

WeatherPrediction::WeatherPrediction(QByteArray bytes) {
    QJsonDocument js = QJsonDocument::fromJson(bytes);

//    extCurrentTemperature = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["Temperature"].toDouble();
//    extCurrentMinTemperature = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["min_temp"].toDouble();
//    extCurrentMaxTemperature = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["max_temp"].toDouble();
//    extCurrentHumidity = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["Humidity"].toInt();
//    extCurrentMinTemperatureTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["date_min_temp"].toInt();
//    extCurrentMaxTemperatureTime = js["body"]["devices"][0]["modules"][0]["dashboard_data"]["date_max_temp"].toInt();

    _longitude = js["city"]["coord"]["lon"].toDouble();
    _latitude = js["city"]["coord"]["lat"].toDouble();

    _sunrise = QDateTime::fromSecsSinceEpoch(js["city"]["sunrise"].toInt());
    _sunset = QDateTime::fromSecsSinceEpoch(js["city"]["sunset"].toInt());
}

double WeatherPrediction::longitude() {return _longitude;}
double WeatherPrediction::latitude() {return _latitude;}

QDateTime WeatherPrediction::sunrise() {return _sunrise;}
QDateTime WeatherPrediction::sunset() {return _sunset;}
