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

    if (indoor) connect(_apiHandler, SIGNAL(indoorRecordListRetrieved(QList<TimestampRecord>)),
                        SLOT(drawChart(QList<TimestampRecord>)));
    else connect(_apiHandler, SIGNAL(outdoorRecordListRetrieved(QList<TimestampRecord>)),
                 SLOT(drawChart(QList<TimestampRecord>)));
}

void HomePageChart::gatherChartData(QString accessToken, QString measurementType, bool indoor, int durationInHours) {
    _measurementType = measurementType;
    if (indoor) {
        _apiHandler->postIndoorChartRequest(
                    QDateTime::currentDateTime().toSecsSinceEpoch() - durationInHours * 3600 - 600,
                    "max",
                    accessToken);
    }
    else {
        _apiHandler->postOutdoorChartRequest(
                    QDateTime::currentDateTime().toSecsSinceEpoch() - durationInHours * 3600 - 600,
                    "max",
                    accessToken);
    }
}

void HomePageChart::drawChart(QList<TimestampRecord> records) {
    QList<QPointF> points = QList<QPointF>();
    for (TimestampRecord record : records) {
        if (_measurementType == "temperature") {
            points.append(QPointF(1000 * record.timestamp(), record.temperature()));
        }
        else if (_measurementType == "humidity") {
            points.append(QPointF(1000 * record.timestamp(), record.humidity()));
        }
        else if (_measurementType == "dewPoint") {
            points.append(QPointF(1000 * record.timestamp(), record.dewPoint()));
        }
        else if (_measurementType == "humidex") {
            points.append(QPointF(1000 * record.timestamp(), record.humidex()));
        }
    }
    drawChart(points);
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

    if (_measurementType == "temperature") {
        yAxis->setLabelFormat(QString("%.1f") + " °C");
    }
    else if (_measurementType == "humidity") {
        yAxis->setLabelFormat(QString("%.0f") + " %");
    }
    else if (_measurementType == "dewPoint") {
        yAxis->setLabelFormat(QString("%.1f") + " °C");
    }
    else if (_measurementType == "humidex") {
        yAxis->setLabelFormat(QString("%.1f") + "");
    }

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
    if (difference < 0.7 && _measurementType != "humidity") {
        yAxis->setTickInterval(0.1);
    }
    else if (difference < 1.3 && _measurementType != "humidity") {
        yAxis->setTickInterval(0.2);
    }
    else if (difference < 2.0 && _measurementType != "humidity") {
        yAxis->setTickInterval(0.4);
    }
    else if (difference < 3.1 && _measurementType != "humidity") {
        yAxis->setTickInterval(0.5);
    }
    else if (difference < 6.1) {
        yAxis->setTickInterval(1.0);
    }
    else if (difference < 12) {
        yAxis->setTickInterval(2.0);
    }
    else if (difference < 15 && _measurementType != "humidity") {
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
