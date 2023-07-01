#include "ColorUtils.h"

ColorUtils::ColorUtils()
{

}

QColor ColorUtils::temperatureColor(double temperature) {

    //t valant 10 fois la température exprimée en °C
    //t =-200 : QColor(0, 0, 0)
    //t =-150 : QColor(128, 0, 128)
    //t =-100 : QColor(128, 0, 255)
    //t = -50 : QColor(0, 0, 255)
    //t =   0 : QColor(0, 128, 255)
    //t =  50 : QColor(0, 255, 0)
    //t = 100 : QColor(255, 255, 0)
    //t = 200 : QColor(255, 128, 0)
    //t = 300 : QColor(255, 0, 0)
    //t = 350 : QColor(255, 0, 128)
    //t = 400 : QColor(255, 0, 255)
    //t = 450 : QColor(128, 0, 128)
    //t = 500 : QColor(0, 0, 0)

    int t = int(10 * temperature + 0.5 - (temperature < 0.0));

    if (t < -200) return QColor(0, 0, 0);
    if (t == -200) return QColor(0, 0, 0);
    if (t < -150) return QColor(10 + (t + 200) * 108 / 50, 0, 10 + (t + 200) * 108 / 50);
    if (t == -150) return QColor(128, 0, 128);
    if (t < -100) return QColor(128, 0, 138 + (t + 150) * 107 / 50);
    if (t == -100) return QColor(128, 0, 255);
    if (t < -50) return QColor(118 + (-100 - t) * 108 / 50, 0, 255);
    if (t == -50) return QColor(0, 0, 255);
    if (t < 0) return QColor(0, 10 + (t + 50) * 108 / 50, 255);
    if (t == 0) return QColor(0, 128, 255);
    if (t < 50) return QColor(0, 138 + t * 107 / 50, 245 - t * 235 / 50);
    if (t == 50) return QColor(0, 255, 0);
    if (t < 100) return QColor(10 + (t - 50) * 235 / 50, 255, 0);
    if (t == 100) return QColor(255, 255, 0);
    if (t < 200) return QColor(255, 245 + (100 - t) * 107 / 100, 0);
    if (t == 200) return QColor(255, 128, 0);
    if (t < 300) return QColor(255, 118 + (200 - t) * 108 / 100, 0);
    if (t == 300) return QColor(255, 0, 0);
    if (t < 350) return QColor(255, 0, 10 + (t - 300) * 108 / 50);
    if (t == 350) return QColor(255, 0, 128);
    if (t < 400) return QColor(255, 0, 138 + (t - 350) * 107 / 50);
    if (t == 400) return QColor(255, 0, 255);
    if (t < 450) return QColor(245 + (400 - t) * 107 / 50, 0, 245 + (400 - t) * 107 / 50);
    if (t == 450) return QColor(128, 0, 128);
    if (t < 500) return QColor(118 + (450 - t) * 108 / 50, 0, 118 + (450 - t) * 108 / 50);
    if (t == 500) return QColor(0, 0, 0);
    return QColor(0, 0, 0);

}

QColor ColorUtils::temperatureColor(QVariant temperature) {
    if (temperature.isNull()) return Qt::white;
    return temperatureColor(temperature.toDouble());
}

QColor ColorUtils::humidityColor(double humidity) {
    return temperatureColor(45 - 0.6 * humidity);
}

QColor ColorUtils::humidityColor(int humidity) {
    return temperatureColor(45 - 0.6 * humidity);
}

QColor ColorUtils::humidityColor(QVariant humidity) {
    if (humidity.isNull()) return Qt::white;
    return humidityColor(humidity.toDouble());
}

QColor ColorUtils::pressureColor(double pressure) {
    if (pressure < 100) { // this is a pressure difference
        return pressureColor(pressure + 990);
    }
    return temperatureColor(-15 + (pressure - 960) * 60 / 126);
}

QColor ColorUtils::pressureColor(QVariant pressure) {
    if (pressure.isNull()) return Qt::white;
    return pressureColor(pressure.toDouble());
}
