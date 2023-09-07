#ifndef BIGPREDICTIONWIDGET_H
#define BIGPREDICTIONWIDGET_H

#include <QLabel>
#include "../../frontend/PredictionWidget/PredictionWidget.h"

class BigPredictionWidget : public PredictionWidget
{
    Q_OBJECT
public:
    BigPredictionWidget();

public slots:
    void setTemperature(double temperature, int i);
    void setPictogram(QString iconId, int i);

private:
    QLabel *temperatureLabels[8];
    QLabel *pictogramLabels[8];
    double _temperatures[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};

#endif // BIGPREDICTIONWIDGET_H