#include "CumulativeChart.h"

extern QColor mainBackgroundColor;
extern int START_YEAR;
extern int BASE_BISSEXTILE_YEAR;

QList<QString> frMonths = {
    "janvier",
    "février",
    "mars",
    "avril",
    "mai",
    "juin",
    "juillet",
    "août",
    "septembre",
    "octobre",
    "novembre",
    "décembre",
};

CumulativeChart::CumulativeChart() {
    aggregator = new CumulativeAggregator(this);

    chart = new QChart();
    chartView = new QChartView();
    chartView->setMinimumWidth(960);

    xAxis = new QCategoryAxis();
    xAxis->setLineVisible(false);
    xAxis->setMin(QDate(BASE_BISSEXTILE_YEAR, 1, 1).toJulianDay() - 0.5);
    xAxis->setMax(QDate(BASE_BISSEXTILE_YEAR + 1, 1, 1).toJulianDay() - 0.5);
    xAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    yAxis = new QCategoryAxis();
    initYAxis();

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
    conditionBox->addItems({"=", "<", "≤", ">", "≥", "≠"});
    conditionBox->setCurrentIndex(1);
    connect(conditionBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    locationBox = new QComboBox();
    locationBox->addItems({"ext.", "int."});
    connect(locationBox, SIGNAL(currentIndexChanged(int)), SLOT(drawChart()));

    includeCurrentYearCheckBox = new QCheckBox("Inclure l'année actuelle dans le calcul de la moyenne");
    includeCurrentYearCheckBox->setChecked(true);
    connect(includeCurrentYearCheckBox, SIGNAL(clicked()), SLOT(drawChart()));

    isCumulativeCheckBox = new QCheckBox("Cumulatif");
    isCumulativeCheckBox->setChecked(true);
    connect(isCumulativeCheckBox, SIGNAL(clicked()), SLOT(drawChart()));

    thresholdLineEdit = new QLineEdit("10");
    connect(thresholdLineEdit, SIGNAL(returnPressed()), SLOT(drawChart()));

    unitLabel = new QLabel("°C");

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
    seasonsLayout->addWidget(fullYearButton, 2, 1, 1, 2);


    layout = new QGridLayout();
    layout->addWidget(chartView, 1, 1, 1, 7);
    layout->addWidget(new QLabel("Année : ", this), 2, 1);
    layout->addWidget(yearBox, 2, 2);
    layout->addWidget(new QLabel("Mois de début : ", this), 2, 3);
    layout->addWidget(startMonthBox, 2, 4);
    layout->addWidget(new QLabel("Mois de fin : ", this), 2, 5);
    layout->addWidget(endMonthBox, 2, 6);
    layout->addWidget(isCumulativeCheckBox, 2, 7);
    layout->addWidget(new QLabel("Grandeur : ", this), 3, 1);
    layout->addWidget(measurementTypeBox, 3, 2);
    layout->addWidget(measurementOptionBox, 3, 3);
    layout->addWidget(locationBox, 3, 4);
    layout->addWidget(new QLabel("Condition : ", this), 4, 1);
    layout->addWidget(conditionBox, 4, 2);
    layout->addWidget(thresholdLineEdit, 4, 3, 1, 3);
    layout->addWidget(unitLabel, 4, 6);
    layout->addWidget(includeCurrentYearCheckBox, 5, 1, 1, 3);
    layout->addLayout(seasonsLayout, 3, 6, 3, 1);
    setLayout(layout);

    // Set pens for all the year series and draw the chart
    yearBox->setCurrentIndex(QDate::currentDate().year() - START_YEAR);

}

void CumulativeChart::setSeriesPens(int emphasizedIndex) {
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
    if (!isCumulativeCheckBox->isChecked()) {
        initYAxis();
        yAxis->setMax(1);
        addTicksToYAxis(1, 0.1);
        return;
    }
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

void CumulativeChart::addTicksToYAxis(int maxOfSeries, double intervalBetweenTicks) {
    for (double i = 0; i <= maxOfSeries; i += intervalBetweenTicks) {
        yAxis->append(QString::number(i, 'f', 1), i);
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

    QMap<int, QList<QPointF>> yearPoints = QMap<int, QList<QPointF>>();
    QList<QPointF> averagePoints = QList<QPointF>();

    QDate minDate = QDate(BASE_BISSEXTILE_YEAR, 1, 1).addMonths(startMonthBox->currentIndex());
    QDate maxDate = QDate(BASE_BISSEXTILE_YEAR, 1, 1).addMonths(endMonthBox->currentIndex() + 1);
    if (endMonthBox->currentIndex() < startMonthBox->currentIndex()) maxDate = maxDate.addYears(1);

    for (int year = START_YEAR; year <= QDate::currentDate().year(); year++) {
        yearPoints[year] = QList<QPointF>();

        QMap<QDate, int> counts = aggregator->countMeasurementsMeetingCriteria(
            measurementTypeBoxToMeasurementType[measurementTypeBox->currentText()],
            measurementOptionBoxToMeasurementOption[measurementOptionBox->currentText()],
            minDate.addYears(year - BASE_BISSEXTILE_YEAR),
            maxDate.addYears(year - BASE_BISSEXTILE_YEAR).addDays(-1),
            conditionBoxToCondition[conditionBox->currentText()],
            indoor,
            isCumulativeCheckBox->isChecked()
        );

        for (auto i = counts.cbegin(), end = counts.cend(); i != end; ++i) {
            QDate date = i.key();
            date = date.addYears(BASE_BISSEXTILE_YEAR - year);
            yearPoints[year].append(QPointF(date.toJulianDay(), i.value()));
        }
    }

    QMap<QDate, double> counts = aggregator->countMeasurementsMeetingCriteriaAveraged(
        measurementTypeBoxToMeasurementType[measurementTypeBox->currentText()],
        measurementOptionBoxToMeasurementOption[measurementOptionBox->currentText()],
        startMonthBox->currentIndex() + 1,
        1,
        QDate(BASE_BISSEXTILE_YEAR, 1, 1).addMonths(endMonthBox->currentIndex() + 1).addDays(-1).month(),
        QDate(BASE_BISSEXTILE_YEAR, 1, 1).addMonths(endMonthBox->currentIndex() + 1).addDays(-1).day(),
        conditionBoxToCondition[conditionBox->currentText()],
        indoor,
        !includeCurrentYearCheckBox->isChecked(),
        isCumulativeCheckBox->isChecked()
    );

    for (auto i = counts.cbegin(), end = counts.cend(); i != end; ++i) {
        QDate date = i.key();

        averagePoints.append(QPointF(date.toJulianDay(), i.value()));
    }

    xAxis->setMin(minDate.toJulianDay() - 0.5);
    xAxis->setMax(maxDate.toJulianDay() - 0.5);

    for (QString label : xAxis->categoriesLabels()) {
        xAxis->remove(label);
    }

    for (
        QDate d = minDate;
        d <= maxDate;
        d = d.addMonths(1)
    ) {
        QString label = d.year() == BASE_BISSEXTILE_YEAR ? d.toString("dd/MM") : " " + d.toString("dd/MM") + " ";
        xAxis->append(label, d.toJulianDay() - 0.5);
    }

    drawChart(yearPoints, averagePoints);
}

void CumulativeChart::drawChart(QMap<int, QList<QPointF>> yearPoints, QList<QPointF> averagePoints) {
    if (chart->axes().length() == 0) {
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
    }
    scaleYAxis(yearPoints);

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

void CumulativeChart::applySpringPeriod() {
    startMonthBox->setCurrentIndex(2);
    endMonthBox->setCurrentIndex(4);
}

void CumulativeChart::applySummerPeriod() {
    startMonthBox->setCurrentIndex(5);
    endMonthBox->setCurrentIndex(7);
}

void CumulativeChart::applyFallPeriod() {
    startMonthBox->setCurrentIndex(8);
    endMonthBox->setCurrentIndex(10);
}

void CumulativeChart::applyWinterPeriod() {
    startMonthBox->setCurrentIndex(11);
    endMonthBox->setCurrentIndex(1);
}

void CumulativeChart::applyFullYearPeriod() {
    startMonthBox->setCurrentIndex(0);
    endMonthBox->setCurrentIndex(11);
}
