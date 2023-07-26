#include "MetricsAnalyzer.h"
#include <algorithm>

extern const QString PATH_TO_COPY_DATABASE;

MetricsAnalyzer::MetricsAnalyzer(QDate date)
{
    _date = date;
    computer = new NormalComputer(new DatabaseHandler(PATH_TO_COPY_DATABASE));
    locale = new QLocale();
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
    double stdevDeltaT = stdevFromMeasurement("(maxTemperature - minTemperature)", tx - tn);
    double stdevRHx = stdevFromMeasurement("maxHumidity", rhx);
    double stdevRHn = stdevFromMeasurement("minHumidity", rhn);
    double stdevDeltaRH = stdevFromMeasurement("(maxHumidity - minHumidity)", rhx - rhn);
    double stdevTdx = stdevFromMeasurement("maxDewPoint", tdx);
    double stdevTdn = stdevFromMeasurement("minDewPoint", tdn);
    double stdevDeltaTd = stdevFromMeasurement("(maxDewPoint - minDewPoint)", tdx - tdn);
    double stdevHx = stdevFromMeasurement("maxHumidex", hx);
    double stdevHn = stdevFromMeasurement("minHumidex", hn);
    double stdevDeltaH = stdevFromMeasurement("(maxHumidex - minHumidex)", hx - hn);

    double values[12] = {
        tx, tn, tx - tn,
        double(rhx), double(rhn), double(rhx - rhn),
        tdx, tdn, tdx - tdn,
        hx, hn, hx - hn
    };

    double standardDeviations[12] = {
        stdevTx, stdevTn, stdevDeltaT,
        stdevRHx, stdevRHn, stdevDeltaRH,
        stdevTdx, stdevTdn, stdevDeltaTd,
        stdevHx, stdevHn, stdevDeltaH
    };

    double absStandardDeviations[12];
    for (int i = 0; i < 12; i++) absStandardDeviations[i] = abs(standardDeviations[i]);

    QStringList measurementsTranslated = {
        "la température maximale", "la température minimale", "la variation de température",
        "l'humidité maximale", "l'humidité minimale", "la variation d'humidité",
        "le point de rosée maximal", "le point de rosée minimal", "la variation de point de rosée",
        "l'humidex maximal", "l'humidex minimal", "la variation de l'humidex"
    };

    int indexOfMostRelevantMetric = *std::max_element(absStandardDeviations, absStandardDeviations + 12);

//    return ("Température maximale : " + QString::number(stdevTx) + " ET" + "\n"
//            "Température minimale : " + QString::number(stdevTn) + " ET" + "\n"
//            "Humidité maximale : " + QString::number(stdevRHx) + " ET" + "\n"
//            "Humidité minimale : " + QString::number(stdevRHn) + " ET" + "\n"
//            "Point de rosée maximal : " + QString::number(stdevTdx) + " ET" + "\n"
//            "Point de rosée minimal : " + QString::number(stdevTdn) + " ET" + "\n"
//            "Humidex maximal : " + QString::number(stdevHx) + " ET" + "\n"
//            "Humidex minimal : " + QString::number(stdevHn) + " ET");

    const int decimals = (3 <= indexOfMostRelevantMetric && indexOfMostRelevantMetric < 6) ? 0 : 1;

    return "Aujourd'hui, la valeur la plus notable est " + measurementsTranslated[indexOfMostRelevantMetric]
            + " de " + locale->toString(values[indexOfMostRelevantMetric], 'f', decimals) + ",\n"
            + "ce qui correspond à un écart de " + locale->toString(standardDeviations[indexOfMostRelevantMetric], 'f', 1)
            + (absStandardDeviations[indexOfMostRelevantMetric] >= 1.95 ? " écarts-types" : " écart-type");

    /*
     * Structure du texte final : "Aujourd'hui, la valeur la plus notable est la variation d'humidité de 40 %,
     * ce qui correspond à un écart de 1,2 écart-type.
     */
}
