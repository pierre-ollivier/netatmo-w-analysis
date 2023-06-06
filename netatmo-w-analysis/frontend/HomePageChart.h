#ifndef HOMEPAGECHART_H
#define HOMEPAGECHART_H

#include <QtCharts>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class HomePageChart : public QChartView
{
public:
    HomePageChart(QString tableName);
    void fillSeries();

private:
    QLineSeries *series;
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;

    DatabaseHandler *dbHandler;

    QString _tableName;

};

#endif // HOMEPAGECHART_H
