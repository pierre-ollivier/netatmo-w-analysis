#ifndef GROWTHCHART_H
#define GROWTHCHART_H

#include <QGridLayout>
#include <QWidget>
#include <QtCharts>
#include "QValueAxis"

#include "../netatmo-w-analysis/backend/CumulativeAggregator.h"

class GrowthChart : public QWidget
{
    Q_OBJECT
public:
    GrowthChart();

    void initYAxis();
    void scaleYAxis(QMap<int, QList<QPointF>> points);
    void addTicksToYAxis(int maxOfSeries, int intervalBetweenTicks);

public slots:
    void drawChart();
    void drawChart(QMap<int, QList<QPointF>> yearPoints, QList<QPointF> averagePoints);

private slots:
    void setUnitLabel(QString measurementType);
    void setSeriesPens(int emphasizedIndex);

private:
    QChart *chart;
    QChartView *chartView;

    QCategoryAxis *xAxis;
    QCategoryAxis *yAxis;

    QComboBox *yearBox;

    QComboBox *measurementTypeBox;
    QComboBox *measurementOptionBox;
    QComboBox *conditionBox;
    QComboBox *locationBox;

    QMap<int, QLineSeries *> *yearSeries;
    QLineSeries *averageSeries;

    QGridLayout *layout;

    CumulativeAggregator *aggregator;
};

#endif // GROWTHCHART_H
