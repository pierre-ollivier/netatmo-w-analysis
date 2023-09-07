#include "PredictionWindow.h"

PredictionWindow::PredictionWindow(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout();

    for (int i = 0; i < 4; i++) {
        predictionWidgets[i] = new BigPredictionWidget();
        layout->addWidget(predictionWidgets[i], 1, i);
    }

    setLayout(layout);
}

PredictionWindow::PredictionWindow(WeatherPrediction *weatherPrediction) : QWidget()
{
    layout = new QGridLayout();

    for (int i = 0; i < 4; i++) {
        predictionWidgets[i] = new BigPredictionWidget();
        layout->addWidget(predictionWidgets[i], 1, i);
    }

    setLayout(layout);
    setWeatherPrediction(weatherPrediction);
}

void PredictionWindow::setWeatherPrediction(WeatherPrediction *weatherPrediction) {
    int mod = QDateTime::currentDateTimeUtc().time().hour() / 3;  // TODO store this information in _weatherPrediction
    _weatherPrediction = weatherPrediction;

    for (int i = 0; i < 4; i++) {
        predictionWidgets[i]->setMaximumTemperature(weatherPrediction->maxTemperature(i + 1));
        predictionWidgets[i]->setMinimumTemperature(weatherPrediction->minTemperature(i + 1));
        predictionWidgets[i]->setMainPictogram(weatherPrediction->dayPictogram(i + 1));
        predictionWidgets[i]->setTitle(QDate::currentDate().addDays(i + 1).toString("d MMMM"));

        for (int j = 0; j < 8; j++) {
            predictionWidgets[i]->setTemperature(weatherPrediction->itemAt(8 * i + j + 7 - mod).temperature(), j);
            predictionWidgets[i]->setPictogram(weatherPrediction->itemAt(8 * i + j + 7 - mod).weatherIcon(), j);
        }
    }
}
