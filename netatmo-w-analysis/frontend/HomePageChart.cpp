#include "HomePageChart.h"
#include <QDateTime>
#include "../types/ExtTimestampRecord.h"

extern QString PATH_TO_COPY_DATABASE;

HomePageChart::HomePageChart(NetatmoAPIHandler *apiHandler, QString tableName, bool indoor) : QChartView()
{
    _tableName = tableName;
    _apiHandler = apiHandler;
    _indoor = indoor;

    locale = new QLocale(QLocale::system());

    xAxis = new QDateTimeAxis();
    xAxis->setFormat("hh:mm");
    xAxis->setTickCount(9);
    xAxis->setLineVisible(false);

    QString unitWithTrailingSpace = " °C";
    yAxis = new QValueAxis();
    yAxis->setLabelFormat(QString("%.1f") + unitWithTrailingSpace);
    yAxis->setTickType(QValueAxis::TicksDynamic);

    series = new QLineSeries();

    chart = new QChart();

    chart->legend()->hide();
    chart->addSeries(series);
    chart->setLocalizeNumbers(true);

    setChart(chart);
    setFixedSize(500, 300);

    if (indoor) connect(_apiHandler, SIGNAL(indoorTemperatureListRetrieved(QList<QPointF>)), SLOT(drawChart(QList<QPointF>)));
    else connect(_apiHandler, SIGNAL(outdoorTemperatureListRetrieved(QList<QPointF>)), SLOT(drawChart(QList<QPointF>)));
}

void HomePageChart::gatherChartData(QString accessToken, bool indoor) {
    if (indoor) {
        _apiHandler->postIndoorChartRequest(
                    QDateTime::currentDateTime().toSecsSinceEpoch() - 4 * 3600 - 600,
                    "max",
                    accessToken);
    }
    else {
        _apiHandler->postOutdoorChartRequest(
                    QDateTime::currentDateTime().toSecsSinceEpoch() - 4 * 3600 - 600,
                    "max",
                    accessToken);
    }
}

void HomePageChart::drawChart(QList<QPointF> points) {
    maxOfSeries = QVariant();
    minOfSeries = QVariant();
    long long maxTimestamp = 0;

    series->clear();
    series->append(points);

    for (QPointF point: points) {
        if (maxOfSeries.isNull() || point.y() > maxOfSeries.toDouble()) maxOfSeries = point.y();
        if (minOfSeries.isNull() || point.y() < minOfSeries.toDouble()) minOfSeries = point.y();
        if (point.x() > maxTimestamp) maxTimestamp = point.x();
    }

    xAxis->setRange(QDateTime::fromMSecsSinceEpoch(maxTimestamp).addMSecs(-7 * 1800000 - maxTimestamp % 1800000),
                    QDateTime::fromMSecsSinceEpoch(maxTimestamp).addMSecs(1800000 - maxTimestamp % 1800000));
    setYAxisRange(maxOfSeries.toDouble(), minOfSeries.toDouble());

    if (chart->axes().length() == 0) {
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
    }

    chart->setLocalizeNumbers(true);

    if (series->attachedAxes().length() == 0) {
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }
}

void HomePageChart::setYAxisRange(double maxValue, double minValue) {
    double difference = maxValue - minValue;
    maxValue += 0.1 * difference;
    minValue -= 0.1 * difference;
    yAxis->setRange(minValue, maxValue);
    setYAxisTicks(maxValue, minValue);
}

void HomePageChart::setYAxisTicks(double maxValue, double minValue) {
    double difference = maxValue - minValue;
    if (difference < 0.7) {
        yAxis->setTickInterval(0.1);
    }
    else if (difference < 1.3) {
        yAxis->setTickInterval(0.2);
    }
    else if (difference < 2.0) {
        yAxis->setTickInterval(0.4);
    }
    else if (difference < 3.1) {
        yAxis->setTickInterval(0.5);
    }
    else if (difference < 6.1) {
        yAxis->setTickInterval(1.0);
    }
    else if (difference < 12) {
        yAxis->setTickInterval(2.0);
    }
    else if (difference < 15) {
        yAxis->setTickInterval(2.5);
    }
    else if (difference < 31) {
        yAxis->setTickInterval(5.0);
    }
    else if (difference < 61) {
        yAxis->setTickInterval(10);
    }
    else {
        yAxis->setTickInterval(20);
    }
}
