#include "NormalsVisualizer.h"

NormalsVisualizer::NormalsVisualizer(NormalComputer *computer) : QWidget()
{
    _computer = computer;

    view = new QChartView();

    xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd/MM");
    xAxis->setTickCount(13);
    xAxis->setLineVisible(false);
    xAxis->setRange(QDateTime(QDate(2020, 1, 1)), QDateTime(QDate(2020, 12, 31)));

    QString unitWithTrailingSpace = " °C";
    yAxis = new QValueAxis();
    yAxis->setLabelFormat(QString("%.1f") + unitWithTrailingSpace);
    yAxis->setRange(0, 30);
    yAxis->setTickType(QValueAxis::TicksDynamic);

//    series = new QLineSeries();

    chart = new QChart();
    chart->legend()->hide();

    seriesMap = new QMap<int, QLineSeries *>();

    for (int stdCount = -2; stdCount <= 2; stdCount++) {
        QLineSeries *series = new QLineSeries();
        seriesMap->insert(stdCount, series);
        chart->addSeries(series);
    }

//    chart->addSeries(series);
    chart->setLocalizeNumbers(true);

    view->setChart(chart);

    temperatureOption = new QRadioButton("Température");
    humidityOption = new QRadioButton("Humidité");
    dewPointOption = new QRadioButton("Point de rosée");
    humidexOption = new QRadioButton("Humidex");
    temperatureOption->setChecked(true);

    connect(temperatureOption, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));
    connect(humidityOption, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));
    connect(dewPointOption, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));
    connect(humidexOption, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));

    maxOption = new QRadioButton("Maximum");
    minOption = new QRadioButton("Minimum");
    avgOption = new QRadioButton("Moyenne");
    maxOption->setChecked(true);

    connect(minOption, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));
    connect(maxOption, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));
    connect(avgOption, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));

    indoorOrOutdoorCheckBox = new QCheckBox("Intérieur");
    connect(indoorOrOutdoorCheckBox, SIGNAL(clicked()), SLOT(changeChartOptions()));

    daysSlider = new QSlider();
    daysSlider->setOrientation(Qt::Horizontal);
    daysSlider->setRange(1, 121);
    daysSlider->setTickInterval(20);
    daysSlider->setTickPosition(QSlider::TicksBelow);
    daysSlider->setValue(41);
    daysSlider->setMinimumWidth(200);
    connect(daysSlider, SIGNAL(valueChanged(int)), SLOT(changeChartOptions()));

    measurementsLayout = new QHBoxLayout();
    measurementsLayout->addWidget(temperatureOption, 0, Qt::AlignCenter);
    measurementsLayout->addWidget(humidityOption, 0, Qt::AlignCenter);
    measurementsLayout->addWidget(dewPointOption, 0, Qt::AlignCenter);
    measurementsLayout->addWidget(humidexOption, 0, Qt::AlignCenter);
    measurementsLayout->addWidget(indoorOrOutdoorCheckBox, 0, Qt::AlignCenter);

    measurementsGroupBox = new QGroupBox("");
    measurementsGroupBox->setLayout(measurementsLayout);

    operationsLayout = new QHBoxLayout();
    operationsLayout->addWidget(maxOption, 1, Qt::AlignCenter);
    operationsLayout->addWidget(minOption, 1, Qt::AlignCenter);
    operationsLayout->addWidget(avgOption, 1, Qt::AlignCenter);
    operationsLayout->addWidget(daysSlider, 2, Qt::AlignCenter);

    operationsGroupBox = new QGroupBox("");
    operationsGroupBox->setLayout(operationsLayout);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(view, 0, 0);
    mainLayout->addWidget(measurementsGroupBox, 1, 0);
    mainLayout->addWidget(operationsGroupBox, 2, 0);
    setLayout(mainLayout);
    changeChartOptions();

}

QList<QPointF> NormalsVisualizer::createChartData(QString tableName,
                                                  QString measurement,
                                                  int daysCount,
                                                  int standardDeviations) {
    QList<QPointF> points = QList<QPointF>();
    for (QDate date = QDate(2020, 1, 1); date.year() < 2021; date = date.addDays(1)) {
        long long x = QDateTime(date).toMSecsSinceEpoch();
        double y = _computer->normalMeasurementByMovingAverage(tableName,
                                                               date,
                                                               measurement,
                                                               daysCount);
        if (standardDeviations != 0) {
            y += standardDeviations * _computer->stdevMeasurementByMovingAverage(tableName,
                                                                                 date,
                                                                                 measurement,
                                                                                 daysCount);
        }
        points.append(QPointF(x, y));
    }
    return points;
}

void NormalsVisualizer::drawChart(QList<QPointF> points) {
    QVariant maxOfSeries = QVariant();
    QVariant minOfSeries = QVariant();

    seriesMap->value(0)->clear();
    seriesMap->value(0)->append(points);

    for (QPointF point: points) {
        if (maxOfSeries.isNull() || point.y() > maxOfSeries.toDouble()) maxOfSeries = point.y();
        if (minOfSeries.isNull() || point.y() < minOfSeries.toDouble()) minOfSeries = point.y();
    }

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

    if (seriesMap->value(0)->attachedAxes().length() == 0) {
        seriesMap->value(0)->attachAxis(xAxis);
        seriesMap->value(0)->attachAxis(yAxis);
    }
}

void NormalsVisualizer::setYAxisRange(double maxValue, double minValue) {
    double difference = maxValue - minValue;
    maxValue += 0.1 * difference;
    minValue -= 0.1 * difference;
    yAxis->setRange(minValue, maxValue);
    setYAxisTicks(maxValue, minValue);
}

void NormalsVisualizer::setYAxisTicks(double maxValue, double minValue) {
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

void NormalsVisualizer::setMeasurementType(QString measurementType) {
    _measurementType = measurementType;
}

void NormalsVisualizer::changeChartOptions() {
    QString tableName = indoorOrOutdoorCheckBox->isChecked() ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    QString measurementType = "";
    if (maxOption->isChecked()) measurementType += "max";
    if (minOption->isChecked()) measurementType += "min";
    if (avgOption->isChecked()) measurementType += "avg";

    if (temperatureOption->isChecked()) {measurementType += "Temperature"; setMeasurementType("temperature");}
    if (humidityOption->isChecked()) {measurementType += "Humidity"; setMeasurementType("humidity");}
    if (dewPointOption->isChecked()) {measurementType += "DewPoint"; setMeasurementType("dewPoint");}
    if (humidexOption->isChecked()) {measurementType += "Humidex"; setMeasurementType("humidex");}

    QList<QPointF> points = createChartData(tableName, measurementType, daysSlider->value());
    drawChart(points);
}
