#ifndef HOMEPAGECHART_H
#define HOMEPAGECHART_H

#include <QtCharts>

class HomePageChart : public QChartView
{
public:
    HomePageChart();

private:
    QLineSeries *series;
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;

};

#endif // HOMEPAGECHART_H
