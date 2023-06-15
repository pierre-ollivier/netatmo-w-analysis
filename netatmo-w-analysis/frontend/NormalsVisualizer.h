#ifndef NORMALSVISUALIZER_H
#define NORMALSVISUALIZER_H

#include <QtCharts>

class NormalsVisualizer : public QChartView
{
public:
    NormalsVisualizer();
    void drawChart(QList<QPointF> points);

    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

private:
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;
    QLineSeries *series;

    QString _measurementType = "temperature";
};

#endif // NORMALSVISUALIZER_H
