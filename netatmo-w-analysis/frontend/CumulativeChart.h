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

    void initYAxis();
    void scaleYAxis(QList<QPointF> points);
    void addTicksToYAxis(int maxOfSeries, int intervalBetweenTicks);

public slots:
    void drawChart();
    void drawChart(QList<QPointF> points);

private:
    QChart *chart;
    QChartView *chartView;

    QCategoryAxis *xAxis;
    QCategoryAxis *yAxis;

    QComboBox *yearBox;

    QComboBox *measurementTypeBox;
    QComboBox *measurementOptionBox;
    QComboBox *conditionBox;

    QLineEdit *thresholdLineEdit;

    QLineSeries *series;

    QGridLayout *layout;

    CumulativeAggregator *aggregator;
};

#endif // CUMULATIVECHART_H
