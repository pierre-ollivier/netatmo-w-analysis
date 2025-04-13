#include "GrowthChart.h"

extern QColor mainBackgroundColor;
extern int START_YEAR;

GrowthChart::GrowthChart() {
    aggregator = new CumulativeAggregator(this);

    chart = new QChart();
    chartView = new QChartView();

    xAxis = new QCategoryAxis();
    xAxis->setLineVisible(false);
    xAxis->setMin(QDate(2024, 1, 1).toJulianDay() - 0.5);
    xAxis->setMax(QDate(2025, 1, 1).toJulianDay() - 0.5);

    for (QDate d = QDate(2024, 1, 1); d <= QDate(2024, 12, 1); d = d.addMonths(1)) {
        xAxis->append(d.toString("dd/MM"), d.toJulianDay() - 0.5);
    }
    xAxis->append("‎01/01\0", QDate(2025, 1, 1).toJulianDay() - 0.5);
    xAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    yAxis = new QCategoryAxis();


    yearBox = new QComboBox();
    for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        yearBox->addItem(QString::number(year));
    }
    connect(yearBox, SIGNAL(currentIndexChanged(int)), SLOT(setSeriesPens(int)));

    measurementTypeBox = new QComboBox();
    measurementTypeBox->addItems({"Température", "Humidité", "Point de rosée", "Humidex", "Pression", "CO2", "Bruit"});
    connect(measurementTypeBox, SIGNAL(currentTextChanged(QString)), SLOT(setUnitLabel(QString)));
    connect(measurementTypeBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    measurementOptionBox = new QComboBox();
    measurementOptionBox->addItems({"min.", "max.", "moy.", "var."});
    connect(measurementOptionBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    conditionBox = new QComboBox();
    conditionBox->addItems({"=", "<", "≤", ">", "≥", "≠"});
    conditionBox->setCurrentIndex(1);
    connect(conditionBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    locationBox = new QComboBox();
    locationBox->addItems({"ext.", "int."});
    connect(locationBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));


    chart = new QChart();
    chart->setLocalizeNumbers(true);

    yearSeries = new QMap<int, QLineSeries *>();

    for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        QLineSeries *series = new QLineSeries();
        series->setName(QString::number(year));

        yearSeries->insert(year, series);
        chart->addSeries(series);
    }
    averageSeries = new QLineSeries();
    averageSeries->setName("Moyenne");
    averageSeries->setPen(QPen(QBrush(Qt::red), 2));
    chart->addSeries(averageSeries);

    chartView->setChart(chart);
    chartView->setBackgroundBrush(QBrush(mainBackgroundColor));

    layout = new QGridLayout();
    layout->addWidget(chartView, 1, 1, 1, 6);
    layout->addWidget(new QLabel("Année : ", this), 2, 1);
    layout->addWidget(yearBox, 2, 2);
    layout->addWidget(new QLabel("Grandeur : ", this), 3, 1);
    layout->addWidget(measurementTypeBox, 3, 2);
    layout->addWidget(measurementOptionBox, 3, 3);
    layout->addWidget(locationBox, 3, 4);
    layout->addWidget(new QLabel("Condition : ", this), 4, 1);
    layout->addWidget(conditionBox, 4, 2);
    setLayout(layout);

    // Set pens for all the year series and draw the chart
    yearBox->setCurrentIndex(QDate::currentDate().year() - START_YEAR);
}
