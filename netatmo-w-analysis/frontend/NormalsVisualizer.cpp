#include "NormalsVisualizer.h"
#include <QPair>

NormalsVisualizer::NormalsVisualizer(NormalComputer *computer) : QWidget()
{
    _computer = computer;

    view = new QChartView();

    xAxis = new QCategoryAxis();
    xAxis->setMin(QDateTime(QDate(2019, 12, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->setMax(QDateTime(QDate(2020, 12, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("J"    , QDateTime(QDate(2020, 1, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("F"    , QDateTime(QDate(2020, 2, 29), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("M"    , QDateTime(QDate(2020, 3, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("A"    , QDateTime(QDate(2020, 4, 30), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append(" M "  , QDateTime(QDate(2020, 5, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append(" J "  , QDateTime(QDate(2020, 6, 30), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("  J  ", QDateTime(QDate(2020, 7, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append(" A "  , QDateTime(QDate(2020, 8, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("S"    , QDateTime(QDate(2020, 9, 30), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("O"    , QDateTime(QDate(2020, 10, 31), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("N"    , QDateTime(QDate(2020, 11, 30), QTime(12, 0)).toMSecsSinceEpoch());
    xAxis->append("D"    , QDateTime(QDate(2020, 12, 31), QTime(12, 0)).toMSecsSinceEpoch());

    QString unitWithTrailingSpace = " °C";
    yAxis = new QValueAxis();
    yAxis->setLabelFormat(QString("%.1f") + unitWithTrailingSpace);
    yAxis->setRange(0, 30);
    yAxis->setTickType(QValueAxis::TicksDynamic);

    chart = new QChart();
    chart->legend()->hide();

    seriesMap = new QMap<double, QLineSeries *>();

    for (int stdCount = -2000; stdCount <= 2000; stdCount += 200) {
        QLineSeries *series = new QLineSeries();
        seriesMap->insert(stdCount, series);
    }

    for (int stdCount = -1900; stdCount < -900; stdCount += 200) {
        colorAreaSeries.insert(stdCount, QColor(0,
                                                51 * (stdCount + 1900) / 200,
                                                255,
                                                80));
    }

    for (int stdCount = -900; stdCount < 100; stdCount += 200) {
        colorAreaSeries.insert(stdCount, QColor(51 * (stdCount + 900) / 200,
                                                255,
                                                255,
                                                80));
    }

    for (int stdCount = 100; stdCount < 1100; stdCount += 200) {
        colorAreaSeries.insert(stdCount, QColor(255,
                                                255,
                                                204 - 51 * (stdCount - 100) / 200,
                                                80));
    }

    for (int stdCount = 1100; stdCount < 2100; stdCount += 200) {
        colorAreaSeries.insert(stdCount, QColor(255,
                                                204 - 51 * (stdCount - 1100) / 200,
                                                0,
                                                80));
    }

    areaSeriesMap = new QMap<double, QAreaSeries *>();

    for (int stdCount = -2000; stdCount < 2000; stdCount += 200) {
        QAreaSeries *areaSeries = new QAreaSeries(seriesMap->value(stdCount + 200), seriesMap->value(stdCount));
        areaSeries->setColor(colorAreaSeries.value(stdCount + 100));
        areaSeries->setBorderColor(colorAreaSeries.value(stdCount + 100));
        areaSeries->setPen(QPen(QBrush(), 0));
        areaSeriesMap->insert(stdCount + 100, areaSeries);
        chart->addSeries(areaSeries);
    }

    drawSeries = QMap<double, bool>();

    for (int stdCount = -2000; stdCount <= 2000; stdCount += 200) {
        drawSeries.insert(stdCount, false);
    }

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

    stdev0Option = new QRadioButton("0");
    stdev1Option = new QRadioButton("1");
    stdev2Option = new QRadioButton("2");
    stdev0Option->setChecked(true);

    connect(stdev0Option, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));
    connect(stdev1Option, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));
    connect(stdev2Option, SIGNAL(clicked(bool)), SLOT(changeChartOptions()));

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

    stdevLayout = new QHBoxLayout();
    stdevLayout->addWidget(new QLabel("Échelle d'affichage : "), 0, Qt::AlignLeft);
    stdevLayout->addWidget(stdev0Option, 0, Qt::AlignCenter);
    stdevLayout->addWidget(stdev1Option, 0, Qt::AlignCenter);
    stdevLayout->addWidget(stdev2Option, 0, Qt::AlignCenter);

    stdevGroupBox = new QGroupBox();
    stdevGroupBox->setLayout(stdevLayout);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(view, 0, 0);
    mainLayout->addWidget(measurementsGroupBox, 1, 0);
    mainLayout->addWidget(operationsGroupBox, 2, 0);
    mainLayout->addWidget(stdevGroupBox, 3, 0);
    setLayout(mainLayout);
    changeChartOptions();

}

QList<QPointF> NormalsVisualizer::createChartData(QList<double> averages,
                                                  QList<double> standardDeviation,
                                                  int standardDeviationsThousands) {
    QList<QPointF> points = QList<QPointF>();
    for (QDate date = QDate(2020, 1, 1); date.year() < 2021; date = date.addDays(1)) {
        long long x = QDateTime(date).toMSecsSinceEpoch();
        double y = averages.value(date.dayOfYear() - 1);
        if (standardDeviationsThousands != 0)
            y += standardDeviationsThousands * 0.001
                    * standardDeviation.value(date.dayOfYear() - 1);

        points.append(QPointF(x, y));
    }
    return points;
}

void NormalsVisualizer::drawChart(QMap<int, QList<QPointF>> pointsMap) {
    QVariant maxOfSeries = QVariant();
    QVariant minOfSeries = QVariant();

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

    drawSeries[-2000] = stdev2Option->isChecked();
    drawSeries[2000] = stdev2Option->isChecked();
    drawSeries[-1000] = stdev2Option->isChecked() || stdev1Option->isChecked();
    drawSeries[1000] = stdev2Option->isChecked() || stdev1Option->isChecked();
    drawSeries[0] = true;

    if (chart->axes().length() == 0) {
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
    }

    chart->setLocalizeNumbers(true);

    for (int stdCount = -2000; stdCount <= 2000; stdCount += 200) {
        if (drawSeries.contains(stdCount) && drawSeries.value(stdCount)) {
            QList<QPointF> points = pointsMap.value(stdCount);
            for (QPointF point: points) {
                if (maxOfSeries.isNull() || point.y() > maxOfSeries.toDouble()) maxOfSeries = point.y();
                if (minOfSeries.isNull() || point.y() < minOfSeries.toDouble()) minOfSeries = point.y();
            }
        }
    }

    for (int stdCount = -2000; stdCount <= 2000; stdCount += 200) {
        QList<QPointF> points = pointsMap.value(stdCount);
        seriesMap->value(stdCount)->clear();
        seriesMap->value(stdCount)->append(points);
    }

    for (int stdCount = -1900; stdCount <= 1900; stdCount += 200) {
        if (areaSeriesMap->value(stdCount)->attachedAxes().length() == 0) {
            areaSeriesMap->value(stdCount)->attachAxis(xAxis);
            areaSeriesMap->value(stdCount)->attachAxis(yAxis);
        }
    }

    setYAxisRange(maxOfSeries.toDouble(), minOfSeries.toDouble());
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

    QMap<int, QList<QPointF>> pointsMap = QMap<int, QList<QPointF>>();

    QList<double> averages = _computer->createAveragesList(tableName,
                                                           measurementType,
                                                           daysSlider->value());
    QList<double> standardDeviation = _computer->createStandardDeviationList(tableName,
                                                                             measurementType,
                                                                             daysSlider->value());
    for (int stdCount = -2000; stdCount <= 2000; stdCount += 200) {
        QList<QPointF> points = createChartData(averages, standardDeviation, stdCount);
        pointsMap.insert(stdCount, points);
    }
    drawChart(pointsMap);
}
