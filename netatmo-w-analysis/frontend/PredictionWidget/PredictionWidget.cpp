#include "PredictionWidget.h"

PredictionWidget::PredictionWidget(QWidget *parent) : QWidget(parent)
{
    maxTemperatureLabel = new QLabel("0 °C");
    minTemperatureLabel = new QLabel("0 °C");

    layout = new QGridLayout();
    layout->addWidget(maxTemperatureLabel, 1, 1);
    layout->addWidget(minTemperatureLabel, 2, 1);

    setLayout(layout);
}

void PredictionWidget::setMaximumTemperature(int maxTemperature) {
    _maxTemperature = maxTemperature;
    maxTemperatureLabel->setText(QString::number(_maxTemperature) + " °C");
}

void PredictionWidget::setMinimumTemperature(int minTemperature) {
    _minTemperature = minTemperature;
    minTemperatureLabel->setText(QString::number(_minTemperature) + " °C");
}
