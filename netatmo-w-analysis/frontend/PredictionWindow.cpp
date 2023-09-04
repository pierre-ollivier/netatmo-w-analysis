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
    layout = new QGridLayout();

    for (int i = 0; i < 4; i++) {
        predictionWidgets[i] = new PredictionWidget();
        layout->addWidget(predictionWidgets[i], 1, i);
    }

    setLayout(layout);
    setWeatherPrediction(weatherPrediction);
}

void PredictionWindow::setWeatherPrediction(WeatherPrediction *weatherPrediction) {
    _weatherPrediction = weatherPrediction;
    for (int i = 0; i < 4; i++) {
        predictionWidgets[i]->setMaximumTemperature(weatherPrediction->maxTemperature(i + 1));
        predictionWidgets[i]->setMinimumTemperature(weatherPrediction->minTemperature(i + 1));
        predictionWidgets[i]->setPictogram(weatherPrediction->dayPictogram(i + 1));
        predictionWidgets[i]->setTitle(QDate::currentDate().addDays(i + 1).toString("d MMMM"));
    }
}
