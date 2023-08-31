#ifndef PREDICTIONWIDGET_H
#define PREDICTIONWIDGET_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QWidget>

class PredictionWidget : public QGroupBox
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
    QLabel *dateLabel;

    QGridLayout *layout;

    int _maxTemperature = 0;
    int _minTemperature = 0;

};

#endif // PREDICTIONWIDGET_H
