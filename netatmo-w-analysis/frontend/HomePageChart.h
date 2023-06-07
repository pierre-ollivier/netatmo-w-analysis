#ifndef HOMEPAGECHART_H
#define HOMEPAGECHART_H

#include <QtCharts>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class HomePageChart : public QChartView
{
public:
    HomePageChart(DatabaseHandler *dbHandler, QString tableName);
    void fillSeries();

private:
    QLineSeries *series;
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;

    QLocale *locale;

    DatabaseHandler *_dbHandler;

    QString _tableName;

    QVariant maxOfSeries = QVariant();
    QVariant minOfSeries = QVariant();

};

#endif // HOMEPAGECHART_H
