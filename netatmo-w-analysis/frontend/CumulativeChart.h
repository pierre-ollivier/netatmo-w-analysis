#ifndef CUMULATIVECHART_H
#define CUMULATIVECHART_H

#include <QGridLayout>
#include <QWidget>
#include <QtCharts>
#include "QValueAxis"

#include "../netatmo-w-analysis/backend/CumulativeAggregator.h"

class CumulativeChart : public QWidget
{
    Q_OBJECT
public:
    CumulativeChart();

    void scaleYAxis(QList<QPointF> points);
    void addTicksToYAxis(int maxOfSeries, int intervalBetweenTicks);
    void drawChart(QList<QPointF> points);

private:
    QChart *chart;
    QChartView *chartView;

    QCategoryAxis *xAxis;
    QCategoryAxis *yAxis;

    QLineSeries *series;

    QGridLayout *layout;

    CumulativeAggregator *aggregator;
};

#endif // CUMULATIVECHART_H
