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
    void setMaximumTemperature(double maxTemperature);
    void setMinimumTemperature(double minTemperature);
    void setPictogram(QString iconId);

private:

    QLabel *maxTemperatureLabel;
    QLabel *minTemperatureLabel;
    QLabel *pictogramLabel;
    QLabel *dateLabel;

    QGridLayout *layout;

    double _maxTemperature = 0;
    double _minTemperature = 0;

};

#endif // PREDICTIONWIDGET_H
