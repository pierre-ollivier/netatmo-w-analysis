#include "CumulativeChart.h"

extern QColor mainBackgroundColor;
extern int START_YEAR;

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
    xAxis->append("‎01/01\0", QDate(2025, 1, 1).toJulianDay());
    xAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    yAxis = new QCategoryAxis();
    initYAxis();

    yearBox = new QComboBox();
    for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        yearBox->addItem(QString::number(year));
    }
    yearBox->setCurrentText(QString::number(QDate::currentDate().year()));
    connect(yearBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

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

    thresholdLineEdit = new QLineEdit("10");
    connect(thresholdLineEdit, SIGNAL(returnPressed()), SLOT(drawChart()));

    unitLabel = new QLabel("°C");

    chart = new QChart();
    chart->setLocalizeNumbers(true);

    yearSeries = new QMap<int, QLineSeries *>();

for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        QLineSeries *series = new QLineSeries();
        series->setName(QString::number(year));
        if (year == QDate::currentDate().year()) {
            series->setMarkerSize(3); // useless - to be removed
        }
        else {
            series->setOpacity(0.25);
        }

        yearSeries->insert(year, series);
        chart->addSeries(series);

    }

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
    layout->addWidget(thresholdLineEdit, 4, 3, 1, 3);
    layout->addWidget(unitLabel, 4, 6);
    setLayout(layout);

    aggregator = new CumulativeAggregator(this);

    drawChart();

}

void CumulativeChart::setUnitLabel(QString measurementType) {
    const QMap<QString, QString> measurementTypeToUnit = {
        {"Température", "°C"},
        {"Humidité", "%"},
        {"Point de rosée", "°C"},
        {"Humidex", ""},
        {"Pression", "hPa"},
        {"CO2", "ppm"},
        {"Bruit", "dB"},
        };
    unitLabel->setText(measurementTypeToUnit[measurementType]);
}

void CumulativeChart::initYAxis() {
    for (QString label: yAxis->categoriesLabels()) {
        yAxis->remove(label);
    }
    yAxis->setLineVisible(false);
    yAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    yAxis->setMin(0);
}

void CumulativeChart::scaleYAxis(QMap<int, QList<QPointF>> points) {
    int maxOfSeries = 0;
    int intervalBetweenTicks = 1;
    for (int year: points.keys()) {
        for (QPointF point: points[year]) {
            if (point.y() > maxOfSeries) maxOfSeries = point.y();
        }
    }

    if (maxOfSeries > 200) {
        maxOfSeries = maxOfSeries + 50 - maxOfSeries % 50;
        intervalBetweenTicks = 50;
    }
    else if (maxOfSeries > 100) {
        maxOfSeries = maxOfSeries + 20 - maxOfSeries % 20;
        intervalBetweenTicks = 20;
    }
    else if (maxOfSeries > 50) {
        maxOfSeries = maxOfSeries + 10 - maxOfSeries % 10;
        intervalBetweenTicks = 10;
    }
    else if (maxOfSeries > 20) {
        maxOfSeries = maxOfSeries + 5 - maxOfSeries % 5;
        intervalBetweenTicks = 5;
    }
    else if (maxOfSeries > 10) {
        maxOfSeries = maxOfSeries + 2 - maxOfSeries % 2;
        intervalBetweenTicks = 2;
    }
    else {
        maxOfSeries = maxOfSeries + 1 - maxOfSeries % 1;
        intervalBetweenTicks = 1;
    }

    initYAxis();
    yAxis->setMax(maxOfSeries);
    addTicksToYAxis(maxOfSeries, intervalBetweenTicks);
}

void CumulativeChart::addTicksToYAxis(int maxOfSeries, int intervalBetweenTicks) {
    for (int i = 0; i <= maxOfSeries; i += intervalBetweenTicks) {
        yAxis->append(QString::number(i), i);
    }
}

void CumulativeChart::drawChart() {
    double threshold = thresholdLineEdit->text().toDouble();

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
    const QMap<QString, std::function<bool(double)>> conditionBoxToCondition = {
        {"=", [threshold](double measurement) {return measurement == threshold;}},
        {"<", [threshold](double measurement) {return measurement < threshold;}},
        {"≤", [threshold](double measurement) {return measurement <= threshold;}},
        {">", [threshold](double measurement) {return measurement > threshold;}},
        {"≥", [threshold](double measurement) {return measurement >= threshold;}},
        {"≠", [threshold](double measurement) {return measurement != threshold;}}
    };
    const bool indoor = locationBox->currentText() == "int." || measurementTypeBox->currentIndex() >= 4;



    // QMap<QDate, double> counts = aggregator->countMeasurementsMeetingCriteriaAveraged(
    //     measurementTypeBoxToMeasurementType[measurementTypeBox->currentText()],
    //     measurementOptionBoxToMeasurementOption[measurementOptionBox->currentText()],
    //     conditionBoxToCondition[conditionBox->currentText()],
    //     indoor
    // );

    QMap<int, QList<QPointF>> points = QMap<int, QList<QPointF>>();

    for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        points[year] = QList<QPointF>();

        QMap<QDate, int> counts = aggregator->countMeasurementsMeetingCriteria(
            measurementTypeBoxToMeasurementType[measurementTypeBox->currentText()],
            measurementOptionBoxToMeasurementOption[measurementOptionBox->currentText()],
            year,
            conditionBoxToCondition[conditionBox->currentText()],
            indoor
        );

        for (auto i = counts.cbegin(), end = counts.cend(); i != end; ++i) {
            QDate date = i.key();
            date.setDate(2024, date.month(), date.day());
            points[year].append(QPointF(date.toJulianDay(), i.value()));
        }
    }

    drawChart(points);
}

void CumulativeChart::drawChart(QMap<int, QList<QPointF>> points) {
    if (chart->axes().length() == 0) {
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
    }
    scaleYAxis(points);

    for (int year : points.keys()) {
        yearSeries->value(year)->clear();
        yearSeries->value(year)->append(points[year]);

        if (yearSeries->value(year)->attachedAxes().length() == 0) {
            yearSeries->value(year)->attachAxis(xAxis);
            yearSeries->value(year)->attachAxis(yAxis);
        }
    }
}
