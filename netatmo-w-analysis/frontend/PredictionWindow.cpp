#include "PredictionWindow.h"

PredictionWindow::PredictionWindow(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout();

    for (int i = 0; i < 4; i++) {
        predictionWidgets[i] = new PredictionWidget();
        layout->addWidget(predictionWidgets[i], 1, i);
    }

    setLayout(layout);
}

PredictionWindow::PredictionWindow(WeatherPrediction *weatherPrediction) : QWidget()
{
    PredictionWindow();
    setWeatherPrediction(weatherPrediction);
}

void PredictionWindow::setWeatherPrediction(WeatherPrediction *weatherPrediction) {
    _weatherPrediction = weatherPrediction;
}
