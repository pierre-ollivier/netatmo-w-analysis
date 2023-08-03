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

    QPair<double, long long> currentMaxTemperatureInfo();
    QPair<double, long long> currentMinTemperatureInfo();

private:
    QDate _date;
    NormalComputer *computer;

    QLocale *locale;

    double mensualLow(QString measurement, int month);
    double mensualHigh(QString measurement, int month);
    double annualLow(QString measurement);
    double annualHigh(QString measurement);

    double currentMinTemperature();
};

#endif // METRICSANALYZER_H
