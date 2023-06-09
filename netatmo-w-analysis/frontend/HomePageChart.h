#ifndef HOMEPAGECHART_H
#define HOMEPAGECHART_H

#include <QtCharts>
#include "../netatmo-w-analysis/backend/NetatmoAPIHandler.h"

class HomePageChart : public QChartView
{
    Q_OBJECT

public:
    HomePageChart(NetatmoAPIHandler *apiHandler, QString tableName, bool indoor);
//    void fillSeries();

public slots:
    void gatherChartData(QString accessToken, bool indoor);
    void drawChart(QList<QPointF> temperatureList);
    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

signals:

private:
    QLineSeries *series;
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;

    QLocale *locale;

    NetatmoAPIHandler *_apiHandler;

    QString _tableName;

    QVariant maxOfSeries = QVariant();
    QVariant minOfSeries = QVariant();

    bool _indoor;

};

#endif // HOMEPAGECHART_H
