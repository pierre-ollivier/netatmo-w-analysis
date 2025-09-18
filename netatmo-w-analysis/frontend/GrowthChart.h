#ifndef GROWTHCHART_H
#define GROWTHCHART_H

#include <QGridLayout>
#include <QWidget>
#include <QtCharts>
#include "QValueAxis"
#include <algorithm>

#include "../netatmo-w-analysis/backend/CumulativeAggregator.h"

class GrowthChart : public QWidget
{
    Q_OBJECT
public:
    GrowthChart();

    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

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
    QValueAxis *yAxis;

    QComboBox *yearBox;

    QComboBox *measurementTypeBox;
    QComboBox *measurementOptionBox;
    QComboBox *conditionBox;
    QComboBox *locationBox;

    QCheckBox *includeMissingConstantValuesCheckBox;
    QCheckBox *includeCurrentYearCheckBox;

    QMap<int, QLineSeries *> *yearSeries;
    QLineSeries *averageSeries;

    QGridLayout *layout;

    CumulativeAggregator *aggregator;
};

#endif // GROWTHCHART_H
