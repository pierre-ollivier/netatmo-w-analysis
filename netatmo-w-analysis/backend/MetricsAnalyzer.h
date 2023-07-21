#ifndef METRICSANALYZER_H
#define METRICSANALYZER_H

#include <QDate>
#include "../netatmo-w-analysis/backend/NormalComputer.h"

class MetricsAnalyzer
{
public:
    MetricsAnalyzer(QDate date = QDate::currentDate());

    double stdevFromMaxTemperature(double maxTemperature);
    double stdevFromMinTemperature(double minTemperature);

private:
    QDate _date;
    NormalComputer *computer;
};

#endif // METRICSANALYZER_H
