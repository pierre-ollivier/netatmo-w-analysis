#ifndef NORMALSVISUALIZER_H
#define NORMALSVISUALIZER_H

#include <QWidget>
#include <QtCharts>
#include <QGroupBox>
#include "../netatmo-w-analysis/backend/NormalComputer.h"

class NormalsVisualizer : public QWidget
{
    Q_OBJECT
public:
    NormalsVisualizer(NormalComputer *computer);

    QList<QPointF> createChartData(QString tableName,
                                   QString measurement,
                                   int daysCount,
                                   int standardDeviations = 0);
    void drawChart(QList<QPointF> points);

    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

    void setMeasurementType(QString measurementType);

public slots:
    void changeChartOptions();

private:
    QChart *chart;
    QChartView *view;
    QDateTimeAxis *xAxis;
    QValueAxis *yAxis;
    QLineSeries *series;

    NormalComputer *_computer;
    QString _measurementType = "temperature";

    QRadioButton *maxOption;
    QRadioButton *minOption;
    QRadioButton *avgOption;
    QHBoxLayout *operationsLayout;
    QGroupBox *operationsGroupBox;

    QRadioButton *temperatureOption;
    QRadioButton *humidityOption;
    QRadioButton *dewPointOption;
    QRadioButton *humidexOption;
    QHBoxLayout *measurementsLayout;
    QGroupBox *measurementsGroupBox;

    QCheckBox *indoorOrOutdoorCheckBox;
    QSlider *daysSlider;

    QGridLayout *mainLayout;
};

#endif // NORMALSVISUALIZER_H
