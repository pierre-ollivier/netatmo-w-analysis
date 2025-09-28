#include "GrowthChart.h"
#include <float.h>

extern QColor mainBackgroundColor;
extern int START_YEAR;
extern int BASE_BISSEXTILE_YEAR;
extern QList<QString> frMonths;

GrowthChart::GrowthChart() {
    aggregator = new CumulativeAggregator(this);

    chart = new QChart();
    chartView = new QChartView();

    xAxis = new QCategoryAxis();
    xAxis->setLineVisible(false);
    xAxis->setMin(QDate(BASE_BISSEXTILE_YEAR, 1, 1).toJulianDay() - 0.5);
    xAxis->setMax(QDate(BASE_BISSEXTILE_YEAR + 1, 1, 1).toJulianDay() - 0.5);

    for (
        QDate d = QDate(BASE_BISSEXTILE_YEAR, 1, 1);
        d <= QDate(BASE_BISSEXTILE_YEAR, 12, 1);
        d = d.addMonths(1)
    ) {
        xAxis->append(d.toString("dd/MM"), d.toJulianDay() - 0.5);
    }
    xAxis->append("‎01/01\0", QDate(2025, 1, 1).toJulianDay() - 0.5);
    xAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    QString unitWithTrailingSpace = " °C";
    yAxis = new QValueAxis();
    yAxis->setLabelFormat(QString("%.1f") + unitWithTrailingSpace);
    yAxis->setTickType(QValueAxis::TicksDynamic);


    yearBox = new QComboBox();
    for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        yearBox->addItem(QString::number(year));
    }
    connect(yearBox, SIGNAL(currentIndexChanged(int)), SLOT(setSeriesPens(int)));

    startMonthBox = new QComboBox();
    endMonthBox = new QComboBox();

    startMonthBox->addItems(frMonths);
    endMonthBox->addItems(frMonths);

    startMonthBox->setCurrentText("janvier");
    endMonthBox->setCurrentText("décembre");

    connect(startMonthBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));
    connect(endMonthBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    measurementTypeBox = new QComboBox();
    measurementTypeBox->addItems({"Température", "Humidité", "Point de rosée", "Humidex", "Pression", "CO2", "Bruit"});
    connect(measurementTypeBox, SIGNAL(currentTextChanged(QString)), SLOT(setUnitLabel(QString)));
    connect(measurementTypeBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    measurementOptionBox = new QComboBox();
    measurementOptionBox->addItems({"min.", "max.", "moy.", "var."});
    connect(measurementOptionBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    conditionBox = new QComboBox();
    conditionBox->addItems({"Minimum", "Maximum", "Moyenne"});
    conditionBox->setCurrentIndex(1);
    connect(conditionBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    locationBox = new QComboBox();
    locationBox->addItems({"ext.", "int."});
    connect(locationBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    includeMissingConstantValuesCheckBox = new QCheckBox("Inclure les valeurs manquantes constantes");
    includeMissingConstantValuesCheckBox->setChecked(true);
    connect(includeMissingConstantValuesCheckBox, SIGNAL(clicked()), SLOT(drawChart()));

    includeCurrentYearCheckBox = new QCheckBox("Inclure l'année actuelle dans le calcul de la moyenne");
    includeCurrentYearCheckBox->setChecked(true);
    connect(includeCurrentYearCheckBox, SIGNAL(clicked()), SLOT(drawChart()));

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

    springButton = new QPushButton("Printemps");
    connect(springButton, SIGNAL(clicked()), SLOT(applySpringPeriod()));
    summerButton = new QPushButton("Été");
    connect(summerButton, SIGNAL(clicked()), SLOT(applySummerPeriod()));
    fallButton = new QPushButton("Automne");
    connect(fallButton, SIGNAL(clicked()), SLOT(applyFallPeriod()));
    winterButton = new QPushButton("Hiver");
    connect(winterButton, SIGNAL(clicked()), SLOT(applyWinterPeriod()));
    fullYearButton = new QPushButton("Année complète");
    connect(fullYearButton, SIGNAL(clicked()), SLOT(applyFullYearPeriod()));

    seasonsLayout = new QGridLayout();
    seasonsLayout->addWidget(springButton, 0, 0);
    seasonsLayout->addWidget(summerButton, 0, 1);
    seasonsLayout->addWidget(fallButton, 1, 0);
    seasonsLayout->addWidget(winterButton, 1, 1);
    seasonsLayout->addWidget(fullYearButton, 2, 0, 1, 2);

    layout = new QGridLayout();
    layout->addWidget(chartView, 1, 1, 1, 6);
    layout->addWidget(new QLabel("Année : ", this), 2, 1);
    layout->addWidget(yearBox, 2, 2);
    layout->addWidget(new QLabel("Grandeur : ", this), 3, 1);
    layout->addWidget(new QLabel("Mois de début : ", this), 2, 3);
    layout->addWidget(startMonthBox, 2, 4);
    layout->addWidget(new QLabel("Mois de fin : ", this), 2, 5);
    layout->addWidget(endMonthBox, 2, 6);
    layout->addWidget(measurementTypeBox, 3, 2);
    layout->addWidget(measurementOptionBox, 3, 3);
    layout->addWidget(locationBox, 3, 4);
    layout->addWidget(new QLabel("Condition : ", this), 4, 1);
    layout->addWidget(conditionBox, 4, 2);
    layout->addWidget(includeMissingConstantValuesCheckBox, 5, 1, 1, 4);
    layout->addWidget(includeCurrentYearCheckBox, 6, 1, 1, 4);
    layout->addLayout(seasonsLayout, 3, 6, 3, 1);
    setLayout(layout);

    // Set pens for all the year series and draw the chart
    yearBox->setCurrentIndex(QDate::currentDate().year() - START_YEAR);
}

double maxOfVector(std::vector<QVariant> vector) {
    if (vector.size() == 0) {
        qDebug() << "Computing the maximum of empty vector, returning 0...";
        return 0.0;
    }
    double result = -DBL_MAX;
    for (QVariant variant: vector) {
        if (!variant.isNull() && result < variant.toDouble()) result = variant.toDouble();
    }
    return result;
}

double minOfVector(std::vector<QVariant> vector) {
    if (vector.size() == 0) {
        qDebug() << "Computing the minimum of empty vector, returning 0...";
        return 0.0;
    }
    double result = DBL_MAX;
    for (QVariant variant: vector) {
        if (!variant.isNull() && result > variant.toDouble()) result = variant.toDouble();
    }
    return result;
}

double averageOfVector(std::vector<QVariant> vector) {
    if (vector.size() == 0) {
        qDebug() << "Computing the average of empty vector, returning 0...";
        return 0.0;
    }
    double cumulatedResult = 0;
    int numberOfNotNullValues = 0;

    for (QVariant variant: vector) {
        if (!variant.isNull()) {
            cumulatedResult += variant.toDouble();
            numberOfNotNullValues++;
        }
    }
    if (numberOfNotNullValues == 0) {
        qDebug() << "Computing the average of vector containing only null values, returning 0...";
        return 0.0;
    }
    return cumulatedResult / numberOfNotNullValues;
}


void GrowthChart::setSeriesPens(int emphasizedIndex) {
    for (int year : yearSeries->keys()) {
        if (year == START_YEAR + emphasizedIndex) {
            yearSeries->value(year)->setPen(QPen(QBrush(Qt::blue), 2));
        }
        else {
            int grayLevel = 208 - 160 * (year - START_YEAR) / (QDate::currentDate().year() - START_YEAR);
            QColor color = QColor(grayLevel, grayLevel, grayLevel);
            yearSeries->value(year)->setPen(QPen(QBrush(color), 1));
        }
    }
    drawChart();
}

void GrowthChart::setUnitLabel(QString measurementType) {
    const QMap<QString, QString> measurementTypeToUnit = {
                                                          {"Température", "°C"},
                                                          {"Humidité", "%"},
                                                          {"Point de rosée", "°C"},
                                                          {"Humidex", ""},
                                                          {"Pression", "hPa"},
                                                          {"CO2", "ppm"},
                                                          {"Bruit", "dB"},
                                                          };
    yAxis->setLabelFormat(QString("%.1f") + " " + measurementTypeToUnit[measurementType]);
}

void GrowthChart::setYAxisRange(double maxValue, double minValue) {
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

void GrowthChart::setYAxisTicks(double maxValue, double minValue) {
    double difference = maxValue - minValue;

    if (difference < 0.7) {
        yAxis->setTickInterval(0.1);
    }
    else if (difference < 1.3) {
        yAxis->setTickInterval(0.2);
    }
    else if (difference < 2.0) {
        yAxis->setTickInterval(0.4);
    }
    else if (difference < 3.1) {
        yAxis->setTickInterval(0.5);
    }
    else if (difference < 6.1) {
        yAxis->setTickInterval(1.0);
    }
    else if (difference < 12) {
        yAxis->setTickInterval(2.0);
    }
    else if (difference < 15) {
        yAxis->setTickInterval(2.5);
    }
    else if (difference < 31) {
        yAxis->setTickInterval(5.0);
    }
    else if (difference < 61) {
        yAxis->setTickInterval(10);
    }
    else if (difference < 121) {
        yAxis->setTickInterval(20);
    }
    else if (difference < 301) {
        yAxis->setTickInterval(50);
    }
    else if (difference < 601) {
        yAxis->setTickInterval(100);
    }
    else if (difference < 1201) {
        yAxis->setTickInterval(200);
    }
    else {
        yAxis->setTickInterval(500);
    }
}

void GrowthChart::drawChart() {
    const QMap<QString, QString> measurementTypeBoxToMeasurementType = {
                                                                        {"Température", "temperature"},
                                                                        {"Humidité", "humidity"},
                                                                        {"Point de rosée", "dewpoint"},
                                                                        {"Humidex", "humidex"},
                                                                        {"Pression", "pressure"},
                                                                        {"CO2", "co2"},
                                                                        {"Bruit", "noise"},
                                                                        };
    const QMap<QString, QString> measurementOptionBoxToMeasurementOption = {
                                                                            {"max.", "max"},
                                                                            {"min.", "min"},
                                                                            {"moy.", "avg"},
                                                                            {"var.", "diff"},
                                                                            };

    const QMap<QString, std::function<double(std::vector<QVariant>)>> aggregationFunctions = {
        {"Maximum", maxOfVector},
        {"Minimum", minOfVector},
        {"Moyenne", averageOfVector}
    };

    const bool indoor = locationBox->currentText() == "int." || measurementTypeBox->currentIndex() >= 4;

    QMap<int, QList<QPointF>> yearPoints = QMap<int, QList<QPointF>>();
    QList<QPointF> averagePoints = QList<QPointF>();

    QMap<int, QMap<QDate, double>> valuesByYear = QMap<int, QMap<QDate, double>>();

    for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        yearPoints[year] = QList<QPointF>();
        QDate startDate = QDate(year, 1, 1).addMonths(startMonthBox->currentIndex());
        QDate endDate = QDate(year, 1, 1).addMonths(endMonthBox->currentIndex() + 1).addDays(-1);
        if (startDate > endDate) endDate = endDate.addYears(1);

        valuesByYear[year] = aggregator->aggregateMeasurements(
            measurementTypeBoxToMeasurementType[measurementTypeBox->currentText()],
            measurementOptionBoxToMeasurementOption[measurementOptionBox->currentText()],
            startDate,
            endDate,
            indoor,
            aggregationFunctions[conditionBox->currentText()],
            includeMissingConstantValuesCheckBox->isChecked()
        );

        for (auto i = valuesByYear[year].cbegin(), end = valuesByYear[year].cend(); i != end; ++i) {
            QDate date = i.key();
            date.setDate(BASE_BISSEXTILE_YEAR, date.month(), date.day());
            yearPoints[year].append(QPointF(date.toJulianDay(), i.value()));
        }
    }

    QMap<QDate, double> averageValues = aggregator->aggregateMeasurementsAveraged(
        valuesByYear,
        includeCurrentYearCheckBox->isChecked()
    );

    for (
        QDate date = QDate(BASE_BISSEXTILE_YEAR, 1, 1);
        date <= QDate(BASE_BISSEXTILE_YEAR, 12, 31);
        date = date.addDays(1)
    ) {
        if (averageValues.contains(date)) averagePoints.append(QPointF(date.toJulianDay(), averageValues[date]));
    }

    drawChart(yearPoints, averagePoints);
}

void GrowthChart::drawChart(QMap<int, QList<QPointF>> yearPoints, QList<QPointF> averagePoints) {
    for (int year: yearPoints.keys()) {
        std::sort(yearPoints[year].begin(), yearPoints[year].end(),
                  [](const QPointF &a, const QPointF &b) {
                      return a.x() < b.x();
                  });
    }
    std::sort(averagePoints.begin(), averagePoints.end(),
              [](const QPointF &a, const QPointF &b) {
                  return a.x() < b.x();
              });

    if (chart->axes().length() == 0) {
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
    }

    double maxOfSeries = -DBL_MAX, minOfSeries = DBL_MAX;
    for (int year: yearPoints.keys()) {
        for (QPointF point: yearPoints[year]) {
            if (point.y() > maxOfSeries) maxOfSeries = point.y();
            if (point.y() < minOfSeries) minOfSeries = point.y();
        }
    }

    setYAxisRange(maxOfSeries, minOfSeries);

    for (int year : yearPoints.keys()) {
        yearSeries->value(year)->clear();
        yearSeries->value(year)->append(yearPoints[year]);

        if (yearSeries->value(year)->attachedAxes().length() == 0) {
            yearSeries->value(year)->attachAxis(xAxis);
            yearSeries->value(year)->attachAxis(yAxis);
        }
    }

    averageSeries->clear();
    averageSeries->append(averagePoints);

    if (averageSeries->attachedAxes().length() == 0) {
        averageSeries->attachAxis(xAxis);
        averageSeries->attachAxis(yAxis);
    }
}

void GrowthChart::applySpringPeriod() {
    startMonthBox->setCurrentIndex(2);
    endMonthBox->setCurrentIndex(4);
}

void GrowthChart::applySummerPeriod() {
    startMonthBox->setCurrentIndex(5);
    endMonthBox->setCurrentIndex(7);
}

void GrowthChart::applyFallPeriod() {
    startMonthBox->setCurrentIndex(8);
    endMonthBox->setCurrentIndex(10);
}

void GrowthChart::applyWinterPeriod() {
    startMonthBox->setCurrentIndex(11);
    endMonthBox->setCurrentIndex(1);
}

void GrowthChart::applyFullYearPeriod() {
    startMonthBox->setCurrentIndex(0);
    endMonthBox->setCurrentIndex(11);
}
