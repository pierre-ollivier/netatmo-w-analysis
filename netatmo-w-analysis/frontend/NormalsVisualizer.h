#ifndef NORMALSVISUALIZER_H
#define NORMALSVISUALIZER_H

#include <QWidget>
#include <QtCharts>
#include <QGroupBox>
#include <QComboBox>
#include "../netatmo-w-analysis/backend/NormalComputer.h"

class NormalsVisualizer : public QWidget
{
    Q_OBJECT
public:
    NormalsVisualizer(NormalComputer *computer);

    QList<QPointF> createChartData(QList<double> averages,
                                   QList<double> standardDeviation = QList<double>(),
                                   int standardDeviationsThousands = 0);

    QList<QPointF> createGivenYearData(int year, QString tableName, QString measurement);

    void drawChart(QMap<int, QList<QPointF>> pointsMap, QList<QPointF> currentYearPoints);

    void setYAxisRange(double maxValue, double minValue);
    void setYAxisTicks(double maxValue, double minValue);

    void setMeasurementType(QString measurementType);

public slots:
    void changeChartOptions();

private:
    int firstYear;

    QChart *chart;
    QChartView *view;
    QCategoryAxis *xAxis;
    QValueAxis *yAxis;

    QMap<double, QLineSeries *> *seriesMap;
    QMap<double, bool> drawSeries;

    QMap<double, QAreaSeries *> *areaSeriesMap;
    QMap<double, QColor> colorAreaSeries;

    QLineSeries *currentYearSeries;

    NormalComputer *_computer;
    QString _measurementType = "temperature";

    QRadioButton *maxOption;
    QRadioButton *minOption;
    QRadioButton *avgOption;
    QRadioButton *varOption;
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

    QComboBox *yearComboBox;
    QHBoxLayout *yearChoiceLayout;

    QCheckBox *indoorOrOutdoorCheckBox;
    QSlider *daysSlider;

    QGridLayout *mainLayout;
};

#endif // NORMALSVISUALIZER_H
