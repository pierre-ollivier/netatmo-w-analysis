#ifndef CUMULATIVECHART_H
#define CUMULATIVECHART_H

#include <QGridLayout>
#include <QWidget>
#include <QtCharts>

#include "../netatmo-w-analysis/backend/CumulativeAggregator.h"

class CumulativeChart : public QWidget
{
    Q_OBJECT
public:
    CumulativeChart();

    void scaleYAxis(QList<QPointF> points);
    void drawChart(QList<QPointF> points);

private:
    QChart *chart;
    QChartView *chartView;

    QCategoryAxis *xAxis;
    QValueAxis *yAxis;

    QLineSeries *series;

    QGridLayout *layout;

    CumulativeAggregator *aggregator;
};

#endif // CUMULATIVECHART_H
