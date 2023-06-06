#include "HomePageChart.h"
#include <QDateTime>
#include "../types/ExtTimestampRecord.h"

extern QString PATH_TO_COPY_DATABASE;

HomePageChart::HomePageChart(QString tableName) : QChartView()
{
    _tableName = tableName;

    dbHandler = new DatabaseHandler(PATH_TO_COPY_DATABASE);

    series = new QLineSeries();
    fillSeries();

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

void HomePageChart::fillSeries() {
//    QString timelimit = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch() - 4 * 3600);
    QString timelimit = QString::number(1685556000);
    std::vector<ExtTimestampRecord> records = dbHandler->getExtTimestampRecordsFromDatabase(
                "SELECT * from " + _tableName + " where timestamp >= " + timelimit, 0);
    for (ExtTimestampRecord record: records) {
        series->append(1000 * record.timestamp(), record.temperature());
    }
}
