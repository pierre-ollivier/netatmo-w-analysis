#ifndef PREDICTIONWINDOW_H
#define PREDICTIONWINDOW_H

#include <QColor>
#include <QGridLayout>
#include <QWidget>
#include "../netatmo-w-analysis/types/WeatherPrediction.h"
#include "../netatmo-w-analysis/frontend/PredictionWidget/BigPredictionWidget.h"

class PredictionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PredictionWindow(QWidget *parent = nullptr);
    PredictionWindow(WeatherPrediction *weatherPrediction);
    void setBackgroundColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void setWeatherPrediction(WeatherPrediction *weatherPrediction);

private:
    QGridLayout *layout;
    QColor backgroundColor;
    BigPredictionWidget *predictionWidgets[4];
    WeatherPrediction *_weatherPrediction;

signals:

};

#endif // PREDICTIONWINDOW_H
