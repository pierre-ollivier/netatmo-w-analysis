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

QString MetricsAnalyzer::text(DatabaseHandler *dbHandler) {
    double tx = dbHandler->getResultFromDatabase(
                "SELECT max(temperature) FROM LastOutdoorTimestampRecords "
                "WHERE day = " + QString::number(QDate::currentDate().day())).toDouble();
    double tn = dbHandler->getResultFromDatabase(
                "SELECT MIN(temperature) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toDouble();
    double tdx = dbHandler->getResultFromDatabase(
                "SELECT MAX(dewPoint) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toDouble();
    double tdn = dbHandler->getResultFromDatabase(
                "SELECT MIN(dewPoint) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toDouble();
    double hx = dbHandler->getResultFromDatabase(
                "SELECT MAX(humidex) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toDouble();
    double hn = dbHandler->getResultFromDatabase(
                "SELECT MIN(humidex) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toDouble();
    int rhx = dbHandler->getResultFromDatabase(
                "SELECT MAX(humidity) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toInt();
    int rhn = dbHandler->getResultFromDatabase(
                "SELECT MIN(humidity) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toInt();

    double stdevTx = stdevFromMeasurement("maxTemperature", tx);
    double stdevTn = stdevFromMeasurement("minTemperature", tn);
    double stdevRHx = stdevFromMeasurement("maxHumidity", rhx);
    double stdevRHn = stdevFromMeasurement("minHumidity", rhn);
    double stdevTdx = stdevFromMeasurement("maxDewPoint", tdx);
    double stdevTdn = stdevFromMeasurement("minDewPoint", tdn);
    double stdevHx = stdevFromMeasurement("maxHumidex", hx);
    double stdevHn = stdevFromMeasurement("minHumidex", hn);

    return ("Température maximale : " + QString::number(stdevTx) + " ET" + "\n"
            "Température minimale : " + QString::number(stdevTn) + " ET" + "\n"
            "Humidité maximale : " + QString::number(stdevRHx) + " ET" + "\n"
            "Humidité minimale : " + QString::number(stdevRHn) + " ET" + "\n"
            "Point de rosée maximal : " + QString::number(stdevTdx) + " ET" + "\n"
            "Point de rosée minimal : " + QString::number(stdevTdn) + " ET" + "\n"
            "Humidex maximal : " + QString::number(stdevHx) + " ET" + "\n"
            "Humidex minimal : " + QString::number(stdevHn) + " ET");
}
