#include "HomePageChart.h"
#include <QDateTime>
#include "../types/ExtTimestampRecord.h"

extern const QLocale LOCALE;
extern QString PATH_TO_COPY_DATABASE;
extern QColor mainBackgroundColor;

HomePageChart::HomePageChart(QString tableName, bool indoor) : QChartView()
{
    _tableName = tableName;
    _indoor = indoor;

    locale = new QLocale(LOCALE);

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
    setFixedSize(500, 280);

    setBackgroundBrush(QBrush(mainBackgroundColor));
}

void HomePageChart::drawChart(QList<ExtTimestampRecord> records) {
    QList<QPointF> points = QList<QPointF>();
    for (ExtTimestampRecord record : records) {
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

void HomePageChart::drawChart(QList<IntTimestampRecord> records) {
    QList<QPointF> points = QList<QPointF>();
    for (IntTimestampRecord record : records) {
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
    long long minTimestamp = 0, maxTimestamp = 0;

    for (QPointF point: points) {
        if (point.x() > maxTimestamp) maxTimestamp = point.x();
    }

    int timeBetweenXTicksInMs = _durationInHours * 3600 * 1000 / 8;
    int maxShiftOfMaxTimestamp =
            _durationInHours == 4? 30 * 60 * 1000:
            _durationInHours == 24? 3600 * 1000:
            _durationInHours == 192? 86400 * 1000:
            timeBetweenXTicksInMs;

    maxTimestamp += maxShiftOfMaxTimestamp - maxTimestamp % maxShiftOfMaxTimestamp;

    if (_durationInHours > 48) {
        maxTimestamp -= QDateTime::currentDateTime().offsetFromUtc() * 1000;
    }

    minTimestamp = maxTimestamp - 8 * timeBetweenXTicksInMs;

    xAxis->setRange(QDateTime::fromMSecsSinceEpoch(minTimestamp),
                    QDateTime::fromMSecsSinceEpoch(maxTimestamp));

    for (QPointF point: points) {
        if (point.x() >= minTimestamp && point.x() <= maxTimestamp) {
            if (maxOfSeries.isNull() || point.y() > maxOfSeries.toDouble()) maxOfSeries = point.y();
            if (minOfSeries.isNull() || point.y() < minOfSeries.toDouble()) minOfSeries = point.y();
        }
    }

    series->clear();
    series->append(points);

    setYAxisRange(maxOfSeries.toDouble(), minOfSeries.toDouble());

    if (timeBetweenXTicksInMs >= 1000 * 86400) {
        xAxis->setFormat("dd/MM");
    }
    else xAxis->setFormat("hh:mm");

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
    if (difference < 0.2) {
        difference = 0.2;
        maxValue = (maxValue + minValue + difference) / 2;
        minValue = (maxValue + minValue - difference) / 2;
    }
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

void HomePageChart::setDurationInHours(int durationInHours) {
    _durationInHours = durationInHours;
}

void HomePageChart::setMeasurementType(QString measurementType) {
    _measurementType = measurementType;
}
