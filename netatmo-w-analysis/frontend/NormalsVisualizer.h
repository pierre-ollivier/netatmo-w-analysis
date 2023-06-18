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
                                   double standardDeviations = 0);
    QList<QPointF> createChartData(QList<double> averages,
                                   QList<double> standardDeviation = QList<double>(),
                                   double standardDeviations = 0);

    void drawChart(QMap<int, QList<QPointF>> pointsMap);

    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

    void setMeasurementType(QString measurementType);

public slots:
    void changeChartOptions();

private:
    QChart *chart;
    QChartView *view;
    QCategoryAxis *xAxis;
    QValueAxis *yAxis;

    QMap<int, QLineSeries *> *seriesMap;
    QMap<int, bool> drawSeries;

    QMap<double, QAreaSeries *> *areaSeriesMap;
    QMap<double, QColor> colorAreaSeries;

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

    QRadioButton *stdev0Option;
    QRadioButton *stdev1Option;
    QRadioButton *stdev2Option;
    QHBoxLayout *stdevLayout;
    QGroupBox *stdevGroupBox;

    QCheckBox *indoorOrOutdoorCheckBox;
    QSlider *daysSlider;

    QGridLayout *mainLayout;
};

#endif // NORMALSVISUALIZER_H
