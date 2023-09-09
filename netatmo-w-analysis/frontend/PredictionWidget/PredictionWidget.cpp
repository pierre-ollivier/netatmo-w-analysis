#include "PredictionWidget.h"
#include <QCoreApplication>
#include <QDebug>

extern const QFont defaultTemperatureFont;

PredictionWidget::PredictionWidget(QWidget *parent) : QGroupBox(parent)
{
    _maxTemperatureLabel = new QLabel("0 °C");
    _maxTemperatureLabel->setFont(defaultTemperatureFont);
    _minTemperatureLabel = new QLabel("0 °C");
    _minTemperatureLabel->setFont(defaultTemperatureFont);
    _pictogramLabel = new QLabel();

    _layout = new QGridLayout();
    _layout->addWidget(_maxTemperatureLabel, 1, 1);
    _layout->addWidget(_minTemperatureLabel, 2, 1);
    _layout->addWidget(_pictogramLabel, 1, 2, 2, 2);

    setLayout(_layout);

    setStyleSheet(
                "QGroupBox {"
                "  border: 1px solid gray;"
                "  border-radius: 3px;"
                "  margin-top: 10px;"
                "}"
                "QGroupBox::title {"
                "  subcontrol-origin: margin;"
                "  subcontrol-position: top center;"
                "}");
    setAlignment(Qt::AlignHCenter);
    setFont(defaultTemperatureFont);
    setMinimumHeight(110);
}

void PredictionWidget::setMaximumTemperature(double maxTemperature) {
    _maxTemperature = maxTemperature;
    _maxTemperatureLabel->setText("<font color=\"#ff1000\">↑ </font>"
                                 + QString::number(_maxTemperature, 'f', 0) + " °C");
}

void PredictionWidget::setMinimumTemperature(double minTemperature) {
    _minTemperature = minTemperature;
    _minTemperatureLabel->setText("<font color=\"#0010ff\">↓ </font>"
                                 + QString::number(_minTemperature, 'f', 0) + " °C");
}

void PredictionWidget::setMainPictogram(QString iconId) {
    QString relativeImagePath = QCoreApplication::applicationDirPath()
            + "/../../netatmo-w-analysis/netatmo-w-analysis/images/" + iconId + "@4x.png";

    _pictogramLabel->setPixmap(QPixmap(relativeImagePath));
}

QGridLayout* PredictionWidget::layout() {
    return _layout;
}

QLabel* PredictionWidget::maxTemperatureLabel() {
    return _maxTemperatureLabel;
}

QLabel* PredictionWidget::minTemperatureLabel() {
    return _minTemperatureLabel;
}

QLabel* PredictionWidget::pictogramLabel() {
    return _pictogramLabel;
}
