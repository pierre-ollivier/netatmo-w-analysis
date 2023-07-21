#include "MetricsAnalyzer.h"

extern const QString PATH_TO_COPY_DATABASE;

MetricsAnalyzer::MetricsAnalyzer(QDate date)
{
    _date = date;
    computer = new NormalComputer(new DatabaseHandler(PATH_TO_COPY_DATABASE));
}

double MetricsAnalyzer::stdevFromMeasurement(QString measurementType, double measurementValue) {
    double average = computer->normalMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                measurementType,
                41);

    double stdev = computer->stdevMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                measurementType,
                average,
                41);

    return (measurementValue - average) / stdev;
}
