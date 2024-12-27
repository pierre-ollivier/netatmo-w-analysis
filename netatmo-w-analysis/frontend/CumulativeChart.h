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
    void scaleYAxis(QMap<int, QList<QPointF>> points);
    void addTicksToYAxis(int maxOfSeries, int intervalBetweenTicks);

public slots:
    void drawChart();
    void drawChart(QList<QPointF> points);
    void drawChart(QMap<int, QList<QPointF>> points);

private slots:
    void setUnitLabel(QString measurementType);

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

    QLineEdit *thresholdLineEdit;

    QLabel *unitLabel;

    QLineSeries *series;
    QMap<int, QLineSeries *> *yearSeries;

    QGridLayout *layout;

    CumulativeAggregator *aggregator;
};

#endif // CUMULATIVECHART_H
