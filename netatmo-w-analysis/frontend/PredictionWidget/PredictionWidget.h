#ifndef PREDICTIONWIDGET_H
#define PREDICTIONWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

class PredictionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PredictionWidget(QWidget *parent = nullptr);

public slots:
    void setMaximumTemperature(int maxTemperature);
    void setMinimumTemperature(int minTemperature);

private:
    QLabel *maxTemperatureLabel;
    QLabel *minTemperatureLabel;

    QGridLayout *layout;

    int _maxTemperature = 0;
    int _minTemperature = 0;

};

#endif // PREDICTIONWIDGET_H
