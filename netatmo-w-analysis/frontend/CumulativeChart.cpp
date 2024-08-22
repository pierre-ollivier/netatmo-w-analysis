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

    yAxis = new QCategoryAxis();
    yAxis->setLineVisible(false);
    yAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    yAxis->setMin(0);

    yearBox = new QComboBox();
    for (int year = 2019; year <= QDate::currentDate().year(); year++) {
        yearBox->addItem(QString::number(year));
    }
    connect(yearBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    series = new QLineSeries();

    chart = new QChart();

    chart->legend()->hide();
    chart->addSeries(series);
    chart->setLocalizeNumbers(true);

    chartView->setChart(chart);
    chartView->setBackgroundBrush(QBrush(mainBackgroundColor));

    layout = new QGridLayout();
    layout->addWidget(chartView, 1, 1);
    layout->addWidget(yearBox, 2, 1);
    setLayout(layout);

    setMinimumWidth(1000);

    aggregator = new CumulativeAggregator(this);

}

void CumulativeChart::scaleYAxis(QList<QPointF> points) {
    int maxOfSeries = 0;
    int intervalBetweenTicks = 1;
    for (QPointF point: points) {
        if (point.y() > maxOfSeries) maxOfSeries = point.y();
    }

    if (maxOfSeries > 100) {
        maxOfSeries = maxOfSeries + 20 - maxOfSeries % 20;
        intervalBetweenTicks = 20;
    }
    else if (maxOfSeries > 50) {
        maxOfSeries = maxOfSeries + 10 - maxOfSeries % 10;
        intervalBetweenTicks = 10;
    }
    else {
        maxOfSeries = maxOfSeries + 5 - maxOfSeries % 5;
        intervalBetweenTicks = 5;
    }

    yAxis->setMax(maxOfSeries);
    addTicksToYAxis(maxOfSeries, intervalBetweenTicks);
    // yAxis->setTickCount(1 + maxOfSeries / intervalBetweenTicks);
}

void CumulativeChart::addTicksToYAxis(int maxOfSeries, int intervalBetweenTicks) {
    for (int i = 0; i <= maxOfSeries; i += intervalBetweenTicks) {
        yAxis->append(QString::number(i), i);
    }
}

void CumulativeChart::drawChart() {
    // provisional data

    QMap<QDate, int> counts = aggregator->countMaxTemperaturesHigherOrEqualThanThreshold(yearBox->currentText().toInt(), 15.0);
    QList<QPointF> points = QList<QPointF>();

    for (auto i = counts.cbegin(), end = counts.cend(); i != end; ++i) {
        QDate date = i.key();
        date.setDate(2024, date.month(), date.day());
        points.append(QPointF(date.toJulianDay(), i.value()));
    }

    drawChart(points);
}

void CumulativeChart::drawChart(QList<QPointF> points) {
    series->clear();
    series->append(points);

    scaleYAxis(points);

    if (chart->axes().length() == 0) {
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
    }

    // chart->setLocalizeNumbers(true);

    if (series->attachedAxes().length() == 0) {
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }
}
