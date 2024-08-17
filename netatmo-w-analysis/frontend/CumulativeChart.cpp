#include "CumulativeChart.h"

extern QColor mainBackgroundColor;

CumulativeChart::CumulativeChart() {
    chart = new QChart();
    chartView = new QChartView();

    xAxis = new QCategoryAxis();
    xAxis->setLineVisible(false);
    xAxis->setMin(QDate(2024, 1, 1).toJulianDay());
    xAxis->setMax(QDate(2025, 1, 1).toJulianDay());

    for (QDate d = QDate(2024, 1, 1); d <= QDate(2024, 12, 1); d = d.addMonths(1)) {
        xAxis->append(d.toString("dd/MM"), d.toJulianDay());
    }
    xAxis->append(" 01/01 ", QDate(2025, 1, 1).toJulianDay());
    xAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    yAxis = new QValueAxis();

    series = new QLineSeries();

    chart = new QChart();

    chart->legend()->hide();
    chart->addSeries(series);
    chart->setLocalizeNumbers(true);

    chartView->setChart(chart);
    chartView->setBackgroundBrush(QBrush(mainBackgroundColor));

    layout = new QGridLayout();
    layout->addWidget(chartView, 1, 1);
    setLayout(layout);

    setMinimumWidth(1000);

    // provisional data
    QList<QPointF> points = QList<QPointF>();
    points.append(QPointF(QDate(2024, 1, 1).toJulianDay(), 0));
    points.append(QPointF(QDate(2024, 2, 1).toJulianDay(), 1));
    points.append(QPointF(QDate(2024, 3, 1).toJulianDay(), 1));
    points.append(QPointF(QDate(2024, 4, 1).toJulianDay(), 6));
    points.append(QPointF(QDate(2024, 5, 1).toJulianDay(), 15));

    drawChart(points);
}

void CumulativeChart::scaleYAxis(QList<QPointF> points) {
    int maxOfSeries = 0;
    for (QPointF point: points) {
        if (point.y() > maxOfSeries) maxOfSeries = point.y();
    }

    if (maxOfSeries > 100) maxOfSeries = maxOfSeries + 20 - maxOfSeries % 20;
    else if (maxOfSeries > 50) maxOfSeries = maxOfSeries + 10 - maxOfSeries % 10;
    else maxOfSeries = maxOfSeries + 5 - maxOfSeries % 5;

    yAxis->setMax(maxOfSeries);
}

void CumulativeChart::drawChart(QList<QPointF> points) {
    series->clear();
    series->append(points);

    scaleYAxis(points);

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
