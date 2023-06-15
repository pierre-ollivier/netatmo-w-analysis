#ifndef NORMALSVISUALIZER_H
#define NORMALSVISUALIZER_H

#include <QtCharts>
#include "../netatmo-w-analysis/backend/NormalComputer.h"

class NormalsVisualizer : public QChartView
{
public:
    NormalsVisualizer(NormalComputer *computer);
    void drawChart(QList<QPointF> points);

    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

    void setMeasurementType(QString measurementType);

private:
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;
    QLineSeries *series;

    NormalComputer *_computer;
    QString _measurementType = "temperature";
};

#endif // NORMALSVISUALIZER_H
