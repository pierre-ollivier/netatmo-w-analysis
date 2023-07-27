#ifndef HOMEPAGECHART_H
#define HOMEPAGECHART_H

#include <QtCharts>
#include "../netatmo-w-analysis/backend/RecentDataHandler.h"
//#include "../netatmo-w-analysis/frontend/MainWindow.h"
#include "../types/TimestampRecord.h"

class HomePageChart : public QChartView
{
    Q_OBJECT

public:
    HomePageChart(QString tableName, bool indoor);

    void setDurationInHours(int durationInHours);
    void setMeasurementType(QString measurementType);

public slots:
    void gatherChartData(QString accessToken, QString measurementType, bool indoor, int durationInHours = 4);
    void drawChart(QList<QPointF> temperatureList);
    void drawChart(QList<ExtTimestampRecord> records);
    void drawChart(QList<IntTimestampRecord> records);
    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

signals:

private:
    QLineSeries *series;
    QChart *chart;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;

    QLocale *locale;

    RecentDataHandler *_recentDataHandler;

    QString _tableName;
    QString _measurementType = "temperature";

    QVariant maxOfSeries = QVariant();
    QVariant minOfSeries = QVariant();

//    MainWindow *_parentWindow;

    bool _indoor;

    int _durationInHours = 4;

};

#endif // HOMEPAGECHART_H
