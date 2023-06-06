#include "HomePageChart.h"

HomePageChart::HomePageChart() : QChartView()
{
    series = new QLineSeries();
    series->append(1685993836000, 24.2);
    series->append(1685993936000, 23.8);
    series->append(1685994000000, 24.1);
    series->append(1685994349000, 24.5);
    series->append(1685994353520, 24.4);

    xAxis = new QDateTimeAxis();
    xAxis->setFormat("hh:mm");

    yAxis = new QValueAxis();
    yAxis->setLabelFormat("%.1f");
//    yAxis->setTickType(QValueAxis::TicksDynamic);
    chart = new QChart();

    chart->legend()->hide();
    chart->addAxis(xAxis,  Qt::AlignBottom);
    chart->addSeries(series);
    chart->addAxis(yAxis, Qt::AlignLeft);

    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    setChart(chart);
    setFixedSize(400, 300);
}
