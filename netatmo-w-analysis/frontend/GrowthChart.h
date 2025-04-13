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
