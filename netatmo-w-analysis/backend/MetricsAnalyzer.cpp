#include "MetricsAnalyzer.h"

extern const QString PATH_TO_COPY_DATABASE;

MetricsAnalyzer::MetricsAnalyzer(QDate date)
{
    _date = date;
    computer = new NormalComputer(new DatabaseHandler(PATH_TO_COPY_DATABASE));
}

double MetricsAnalyzer::stdevFromMaxTemperature(double maxTemperature) {
    double average = computer->normalMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                "maxTemperature",
                41);

    double stdev = computer->stdevMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                "maxTemperature",
                average,
                41);

    return (maxTemperature - average) / stdev;
}

double MetricsAnalyzer::stdevFromMinTemperature(double minTemperature) {
    double average = computer->normalMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                "minTemperature",
                41);

    double stdev = computer->stdevMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                "minTemperature",
                average,
                41);

    return (minTemperature - average) / stdev;
}
