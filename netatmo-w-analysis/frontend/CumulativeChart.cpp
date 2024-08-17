#include "CumulativeChart.h"

CumulativeChart::CumulativeChart() {
    chart = new QChart();
    chartView = new QChartView();

    xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd/MM");
    xAxis->setTickCount(13);
    xAxis->setLineVisible(false);
    xAxis->setMin(QDateTime(QDate(2024, 1, 1), QTime(0, 0)));
    xAxis->setMax(QDateTime(QDate(2025, 1, 1), QTime(0, 0)));

    yAxis = new QValueAxis();

    series = new QLineSeries();

    chart = new QChart();

    chart->legend()->hide();
    chart->addSeries(series);
    chart->setLocalizeNumbers(true);
}

void CumulativeChart::drawChart(QList<QPointF> points) {
    series->clear();
    series->append(points);

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
