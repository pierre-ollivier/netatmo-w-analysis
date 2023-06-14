#ifndef NORMALSVISUALIZER_H
#define NORMALSVISUALIZER_H

#include <QtCharts>

class NormalsVisualizer : public QChartView
{
public:
    NormalsVisualizer();
    void drawChart(QList<QPointF> points);

private:
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;
    QLineSeries *series;
};

#endif // NORMALSVISUALIZER_H
