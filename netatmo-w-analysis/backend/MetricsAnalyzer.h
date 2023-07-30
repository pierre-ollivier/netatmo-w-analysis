#ifndef METRICSANALYZER_H
#define METRICSANALYZER_H

#include <QDate>
#include <QLocale>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"
#include "../netatmo-w-analysis/backend/NormalComputer.h"

class MetricsAnalyzer
{
public:
    MetricsAnalyzer(QDate date = QDate::currentDate());

    QString text(DatabaseHandler *dbHandler);
    double stdevFromMeasurement(QString measurementType, double measurementValue, bool currentDateIsUsed = true);

    int indexOfMaxElement(double array[]);

private:
    QDate _date;
    NormalComputer *computer;

    QLocale *locale;
};

#endif // METRICSANALYZER_H
