#include "BigPredictionWidget.h"
#include <QCoreApplication>

BigPredictionWidget::BigPredictionWidget() : PredictionWidget()
{
    for (int i = 0; i < 8; i++) {
        timeLabels[i] = new QLabel("<b>0 h</b>");
        temperatureLabels[i] = new QLabel("0 °C");
        pictogramLabels[i] = new QLabel();
        layout()->addWidget(timeLabels[i], 10 + i, 1);
        layout()->addWidget(temperatureLabels[i], 10 + i, 2);
        layout()->addWidget(pictogramLabels[i], 10 + i, 3);
    }
}

void BigPredictionWidget::setTemperature(double temperature, int i) {
    _temperatures[i] = temperature;
    temperatureLabels[i]->setText("<b>" + QString::number(temperature - 273.15, 'f', 0) + "</b> °C");  // convert to °C
}

void BigPredictionWidget::setPictogram(QString iconId, int i) {
    QString relativeImagePath = QCoreApplication::applicationDirPath()
            + "/../../netatmo-w-analysis/netatmo-w-analysis/images/" + iconId + "@4x.png";

    pictogramLabels[i]->setPixmap(QPixmap(relativeImagePath));
}

void BigPredictionWidget::setDaylightTime(bool isDaylightTime) {
    int offsetFromUTC = isDaylightTime ? 2 : 1;
    for (int h = 0; h <= 21; h += 3) {
        timeLabels[h / 3]->setText(QString::number(h + offsetFromUTC) + " h");
    }
}
