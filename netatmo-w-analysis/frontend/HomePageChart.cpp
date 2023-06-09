#include "HomePageChart.h"
#include <QDateTime>
#include "../types/ExtTimestampRecord.h"

extern QString PATH_TO_COPY_DATABASE;

HomePageChart::HomePageChart(NetatmoAPIHandler *apiHandler, QString tableName) : QChartView()
{
    _tableName = tableName;
    _apiHandler = apiHandler;

    locale = new QLocale(QLocale::system());

    xAxis = new QDateTimeAxis();
    xAxis->setFormat("hh:mm");
//    xAxis->setRange(
//                QDateTime(QDate(2023, 5, 31), QTime(20, 0)),
//                QDateTime(QDate(2023, 6, 1), QTime(0, 0)));
    // provisional - range needs to be adapted to the scale
    xAxis->setTickCount(5);

    QString unitWithTrailingSpace = " °C";
    yAxis = new QValueAxis();
    yAxis->setLabelFormat(QString("%.1f") + unitWithTrailingSpace);
    yAxis->setTickType(QValueAxis::TicksDynamic);

    series = new QLineSeries();
//    fillSeries();

    chart = new QChart();

    chart->legend()->hide();
//    chart->addAxis(xAxis, Qt::AlignBottom);
//    chart->addSeries(series);
//    chart->addAxis(yAxis, Qt::AlignLeft);
    chart->setLocalizeNumbers(true);

//    series->attachAxis(xAxis);
//    series->attachAxis(yAxis);

    setChart(chart);
    setFixedSize(400, 300);

    connect(_apiHandler, SIGNAL(temperatureListRetrieved(QList<QPointF>)), SLOT(drawChart(QList<QPointF>)));
    _apiHandler->postChartRequest(
                QDateTime::currentDateTime().toSecsSinceEpoch() - 4 * 3600,
                "max",
                _apiHandler->getAccessToken());
}

void HomePageChart::gatherChartData(QString accessToken) {
    _apiHandler->postChartRequest(
                QDateTime::currentDateTime().toSecsSinceEpoch() - 4 * 3600,
                "max",
                accessToken);
}

void HomePageChart::drawChart(QList<QPointF> points) {
//    int inttimelimit = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch() - 4 * 3600);
    int inttimelimit = 1685556000;
    int chartStart = inttimelimit - (inttimelimit % 1800);
    QString timelimit = QString::number(inttimelimit);

    maxOfSeries = QVariant();
    minOfSeries = QVariant();

//    for (ExtTimestampRecord record: records) {
//        series->append(1000 * record.timestamp(), record.temperature());
//        if (maxOfSeries.isNull() || record.temperature() > maxOfSeries.toDouble()) maxOfSeries = record.temperature();
//        if (minOfSeries.isNull() || record.temperature() < minOfSeries.toDouble()) minOfSeries = record.temperature();
//    }

    series->clear();
    series->append(points);

    for (QPointF point: points) {
        if (maxOfSeries.isNull() || point.y() > maxOfSeries.toDouble()) maxOfSeries = point.y();
        if (minOfSeries.isNull() || point.y() < minOfSeries.toDouble()) minOfSeries = point.y();
    }

    xAxis->setRange(QDateTime::currentDateTime().addSecs(-4 * 3600),
                    QDateTime::currentDateTime());
    setYAxisRange(maxOfSeries.toDouble(), minOfSeries.toDouble());

    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addSeries(series);
    chart->addAxis(yAxis, Qt::AlignLeft);
    chart->setLocalizeNumbers(true);

    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    setChart(chart);
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
