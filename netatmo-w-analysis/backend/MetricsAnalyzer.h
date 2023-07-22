#ifndef METRICSANALYZER_H
#define METRICSANALYZER_H

#include <QDate>
#include "../netatmo-w-analysis/backend/NormalComputer.h"

class MetricsAnalyzer
{
public:
    MetricsAnalyzer(QDate date = QDate::currentDate());

    double stdevFromMeasurement(QString measurementType, double measurementValue);

private:
    QDate _date;
    NormalComputer *computer;
};

#endif // METRICSANALYZER_H