#ifndef COLORUTILS_H
#define COLORUTILS_H

#include <QColor>
#include <QVariant>

class ColorUtils
{
public:
    ColorUtils();

    static QColor temperatureColor(double temperature);
    static QColor temperatureColor(QVariant temperature);

    static QColor humidityColor(int humidity);
    static QColor humidityColor(double humidity);
    static QColor humidityColor(QVariant humidity);

    static QColor pressureColor(double pressure);
    static QColor pressureColor(QVariant pressure);
};

#endif // COLORUTILS_H
