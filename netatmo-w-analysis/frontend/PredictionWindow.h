#ifndef PREDICTIONWINDOW_H
#define PREDICTIONWINDOW_H

#include <QGridLayout>
#include <QWidget>
#include "../netatmo-w-analysis/types/WeatherPrediction.h"
#include "../netatmo-w-analysis/frontend/PredictionWidget/PredictionWidget.h"

class PredictionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PredictionWindow(QWidget *parent = nullptr);
    PredictionWindow(WeatherPrediction *weatherPrediction);

public slots:
    void setWeatherPrediction(WeatherPrediction *weatherPrediction);

private:
    QGridLayout *layout;
    PredictionWidget *predictionWidgets[4];
    WeatherPrediction *_weatherPrediction;

signals:

};

#endif // PREDICTIONWINDOW_H
