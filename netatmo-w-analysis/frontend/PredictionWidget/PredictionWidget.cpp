#include "PredictionWidget.h"
#include <QCoreApplication>
#include <QDebug>

PredictionWidget::PredictionWidget(QWidget *parent) : QGroupBox(parent)
{
    maxTemperatureLabel = new QLabel("0 °C");
    minTemperatureLabel = new QLabel("0 °C");
    pictogramLabel = new QLabel();

    layout = new QGridLayout();
    layout->addWidget(maxTemperatureLabel, 1, 1);
    layout->addWidget(minTemperatureLabel, 2, 1);
    layout->addWidget(pictogramLabel, 1, 2, 2, 1);

    setLayout(layout);

    setAlignment(Qt::AlignHCenter);

    setPictogram("");
}

void PredictionWidget::setMaximumTemperature(double maxTemperature) {
    _maxTemperature = maxTemperature;
    maxTemperatureLabel->setText("<font color=\"#ff1000\">↑ </font>"
                                 + QString::number(_maxTemperature, 'f', 0) + " °C");
}

void PredictionWidget::setMinimumTemperature(double minTemperature) {
    _minTemperature = minTemperature;
    minTemperatureLabel->setText("<font color=\"#0010ff\">↓ </font>"
                                 + QString::number(_minTemperature, 'f', 0) + " °C");
}

void PredictionWidget::setPictogram(QString iconId) {
    qDebug() << "Setting pictogram...";
    QString relativeImagePath = QCoreApplication::applicationDirPath()
            + "/../../netatmo-w-analysis/netatmo-w-analysis/images/10d@4x.png";
    qDebug() << QPixmap(relativeImagePath);
    pictogramLabel->setPixmap(QPixmap(relativeImagePath));
}
