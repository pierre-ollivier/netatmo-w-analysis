#ifndef PREDICTIONWIDGET_H
#define PREDICTIONWIDGET_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

class PredictionWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit PredictionWidget(QWidget *parent = nullptr);
    QGridLayout* layout();

public slots:
    void setMaximumTemperature(double maxTemperature);
    void setMinimumTemperature(double minTemperature);
    void setMainPictogram(QString iconId);

private:

    QLabel *maxTemperatureLabel;
    QLabel *minTemperatureLabel;
    QLabel *pictogramLabel;
    QLabel *dateLabel;

    QGridLayout *_layout;

    double _maxTemperature = 0;
    double _minTemperature = 0;

};

#endif // PREDICTIONWIDGET_H
