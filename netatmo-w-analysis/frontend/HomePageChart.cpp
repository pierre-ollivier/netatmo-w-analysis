#include "HomePageChart.h"
#include <QDateTime>
#include "../types/ExtTimestampRecord.h"

extern QString PATH_TO_COPY_DATABASE;

HomePageChart::HomePageChart(DatabaseHandler *dbHandler, QString tableName) : QChartView()
{
    _tableName = tableName;
    _dbHandler = dbHandler;

    locale = new QLocale(QLocale::system());

    xAxis = new QDateTimeAxis();
    xAxis->setFormat("hh:mm");
//    xAxis->setRange(
//                QDateTime(QDate(2023, 5, 31), QTime(20, 0)),
//                QDateTime(QDate(2023, 6, 1), QTime(0, 0)));
    // provisional - range needs to be adapted to the scale
    xAxis->setTickCount(5);

    series = new QLineSeries();
    fillSeries();

    QString unit = "";

    yAxis = new QValueAxis();
    yAxis->setLabelFormat(QString("%.1f") + unit);
//    yAxis->setTickType(QValueAxis::TicksDynamic);
    chart = new QChart();

    chart->legend()->hide();
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addSeries(series);
    chart->addAxis(yAxis, Qt::AlignLeft);
    chart->setLocalizeNumbers(true);

    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    setChart(chart);
    setFixedSize(400, 300);
}

void HomePageChart::fillSeries() {
//    int inttimelimit = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch() - 4 * 3600);
    int inttimelimit = 1685556000;
    int chartStart = inttimelimit - (inttimelimit % 1800);
    QString timelimit = QString::number(inttimelimit);
    std::vector<ExtTimestampRecord> records = _dbHandler->getExtTimestampRecordsFromDatabase(
                "SELECT * from " + _tableName + " where timestamp >= " + timelimit, 0);
    maxOfSeries = QVariant();
    minOfSeries = QVariant();

    for (ExtTimestampRecord record: records) {
        series->append(1000 * record.timestamp(), record.temperature());
        if (maxOfSeries.isNull() || record.temperature() > maxOfSeries.toDouble()) maxOfSeries = record.temperature();
        if (minOfSeries.isNull() || record.temperature() < minOfSeries.toDouble()) minOfSeries = record.temperature();
    }

    xAxis->setRange(QDateTime::fromSecsSinceEpoch(chartStart),
                    QDateTime::fromSecsSinceEpoch(chartStart + 4 * 3600));
}
