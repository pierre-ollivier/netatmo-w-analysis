#include "NormalsVisualizer.h"

NormalsVisualizer::NormalsVisualizer() : QChartView()
{
    xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd/MM");
    xAxis->setTickCount(13);
    xAxis->setLineVisible(false);
    xAxis->setRange(QDateTime(QDate(2020, 1, 1)), QDateTime(QDate(2020, 12, 31)));

    QString unitWithTrailingSpace = " °C";
    yAxis = new QValueAxis();
    yAxis->setLabelFormat(QString("%.1f") + unitWithTrailingSpace);
    yAxis->setRange(0, 30);
//    yAxis->setTickType(QValueAxis::TicksDynamic);

    series = new QLineSeries();

    chart = new QChart();

    chart->legend()->hide();
    chart->addSeries(series);
    chart->setLocalizeNumbers(true);

    setChart(chart);
}

void NormalsVisualizer::drawChart(QList<QPointF> points) {
//    maxOfSeries = QVariant();
//    minOfSeries = QVariant();
//    long long minTimestamp = 0, maxTimestamp = 0;

    series->clear();
    series->append(points);

//    for (QPointF point: points) {
//        if (point.x() > maxTimestamp) maxTimestamp = point.x();
//    }

//    int timeBetweenXTicksInMs = _durationInHours * 3600 * 1000 / 8;
//    int maxShiftOfMaxTimestamp =
//            _durationInHours == 4? 30 * 60 * 1000:
//            _durationInHours == 24? 3600 * 1000:
//            _durationInHours == 192? 86400 * 1000:
//            timeBetweenXTicksInMs;

//    maxTimestamp += maxShiftOfMaxTimestamp - maxTimestamp % maxShiftOfMaxTimestamp;

//    if (_durationInHours > 48) {
//        maxTimestamp -= QDateTime::currentDateTime().offsetFromUtc() * 1000;
//    }

//    minTimestamp = maxTimestamp - 8 * timeBetweenXTicksInMs;

//    xAxis->setRange(QDateTime::fromMSecsSinceEpoch(minTimestamp),
//                    QDateTime::fromMSecsSinceEpoch(maxTimestamp));

//    for (QPointF point: points) {
//        if (point.x() >= minTimestamp && point.x() <= maxTimestamp) {
//            if (maxOfSeries.isNull() || point.y() > maxOfSeries.toDouble()) maxOfSeries = point.y();
//            if (minOfSeries.isNull() || point.y() < minOfSeries.toDouble()) minOfSeries = point.y();
//        }
//    }

//    setYAxisRange(maxOfSeries.toDouble(), minOfSeries.toDouble());

//    if (timeBetweenXTicksInMs >= 1000 * 86400) {
//        xAxis->setFormat("dd/MM");
//    }
//    else xAxis->setFormat("hh:mm");

//    if (_measurementType == "temperature") {
//        yAxis->setLabelFormat(QString("%.1f") + " °C");
//    }
//    else if (_measurementType == "humidity") {
//        yAxis->setLabelFormat(QString("%.0f") + " %");
//    }
//    else if (_measurementType == "dewPoint") {
//        yAxis->setLabelFormat(QString("%.1f") + " °C");
//    }
//    else if (_measurementType == "humidex") {
//        yAxis->setLabelFormat(QString("%.1f") + "");
//    }

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
