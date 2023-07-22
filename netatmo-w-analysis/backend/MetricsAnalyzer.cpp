#include "MetricsAnalyzer.h"

extern const QString PATH_TO_COPY_DATABASE;

MetricsAnalyzer::MetricsAnalyzer(QDate date)
{
    _date = date;
    computer = new NormalComputer(new DatabaseHandler(PATH_TO_COPY_DATABASE));
}

double MetricsAnalyzer::stdevFromMeasurement(QString measurementType, double measurementValue) {
    QString table = (measurementType != "maxPressure" && measurementType != "minPressure") ?
                "OutdoorDailyRecords" : "IndoorDailyRecords";
    double average = computer->normalMeasurementByMovingAverage(
                table,
                _date,
                measurementType,
                41);

    double stdev = computer->stdevMeasurementByMovingAverage(
                table,
                _date,
                measurementType,
                average,
                41);

    return (measurementValue - average) / stdev;
}
