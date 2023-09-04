#include "WeatherPredictionContainer.h"
#include <QMouseEvent>

WeatherPredictionContainer::WeatherPredictionContainer(QWidget *parent) : QWidget(parent)
{

}

void WeatherPredictionContainer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}
