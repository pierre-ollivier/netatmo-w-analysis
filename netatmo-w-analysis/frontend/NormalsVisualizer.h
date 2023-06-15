#ifndef NORMALSVISUALIZER_H
#define NORMALSVISUALIZER_H

#include <QWidget>
#include <QtCharts>
#include <QGroupBox>
#include "../netatmo-w-analysis/backend/NormalComputer.h"

class NormalsVisualizer : public QWidget
{
public:
    NormalsVisualizer(NormalComputer *computer);
    void drawChart(QList<QPointF> points);

    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

    void setMeasurementType(QString measurementType);

private:
    QChart *chart;
    QChartView *view;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;
    QLineSeries *series;

    NormalComputer *_computer;
    QString _measurementType = "temperature";

    QRadioButton *maxOption;
    QRadioButton *minOption;
    QRadioButton *avgOption;
    QHBoxLayout *operationsLayout;
    QGroupBox *operationsGroupBox;

    QRadioButton *temperatureOption;
    QRadioButton *humidityOption;
    QRadioButton *dewPointOption;
    QRadioButton *humidexOption;
    QHBoxLayout *measurementsLayout;
    QGroupBox *measurementsGroupBox;

    QGridLayout *mainLayout;
};

#endif // NORMALSVISUALIZER_H
