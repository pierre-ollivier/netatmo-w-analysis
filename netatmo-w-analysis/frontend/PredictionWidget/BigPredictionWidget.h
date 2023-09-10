#ifndef BIGPREDICTIONWIDGET_H
#define BIGPREDICTIONWIDGET_H

#include <QLabel>
#include "../../frontend/PredictionWidget/PredictionWidget.h"

class BigPredictionWidget : public PredictionWidget
{
    Q_OBJECT
public:
    BigPredictionWidget();

public slots:
    void setTemperature(double temperature, int i);
    void setPictogram(QString iconId, int i);
    void setToolTip(QString toolTip, int i);
    void setDaylightTime(bool isDaylightTime);
    void emphasizeDailyValues();

private:
    QLabel *timeLabels[8];
    QLabel *temperatureLabels[8];
    QLabel *pictogramLabels[8];
    QLabel *underscoreLabel;
    double _temperatures[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};

#endif // BIGPREDICTIONWIDGET_H
