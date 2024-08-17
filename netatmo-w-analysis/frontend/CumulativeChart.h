#ifndef CUMULATIVECHART_H
#define CUMULATIVECHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts>

class CumulativeChart : public QWidget
{
    Q_OBJECT
public:
    CumulativeChart();

    void drawChart(QList<QPointF> points);

private:
    QChart *chart;
    QChartView *chartView;

    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;

    QLineSeries *series;
};

#endif // CUMULATIVECHART_H
