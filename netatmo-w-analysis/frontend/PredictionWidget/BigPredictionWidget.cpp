#include "BigPredictionWidget.h"
#include <QCoreApplication>

BigPredictionWidget::BigPredictionWidget() : PredictionWidget()
{
    for (int i = 0; i < 8; i++) {
        temperatureLabels[i] = new QLabel("0 °C");
        pictogramLabels[i] = new QLabel();
        layout()->addWidget(temperatureLabels[i], 10 + i, 1);
        layout()->addWidget(pictogramLabels[i], 10 + i, 2);
    }
}

void BigPredictionWidget::setTemperature(double temperature, int i) {
    _temperatures[i] = temperature;
    temperatureLabels[i]->setText(QString::number(temperature - 273.15, 'f', 0) + " °C");  // convert to °C
}

void BigPredictionWidget::setPictogram(QString iconId, int i) {
    QString relativeImagePath = QCoreApplication::applicationDirPath()
            + "/../../netatmo-w-analysis/netatmo-w-analysis/images/" + iconId + "@4x.png";

    pictogramLabels[i]->setPixmap(QPixmap(relativeImagePath));
}
