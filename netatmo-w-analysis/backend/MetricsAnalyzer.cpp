#include "MetricsAnalyzer.h"
#include <algorithm>
#include <QDebug>

extern const QString PATH_TO_COPY_DATABASE;

MetricsAnalyzer::MetricsAnalyzer(QDate date)
{
    _date = date;
    computer = new NormalComputer(new DatabaseHandler(PATH_TO_COPY_DATABASE));
    locale = new QLocale();
}

double MetricsAnalyzer::stdevFromMeasurement(QString measurementType, double measurementValue) {
    QString table = (!measurementType.contains("pressure", Qt::CaseInsensitive)) ?
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

    double px = dbHandler->getResultFromDatabase(
                "SELECT max(pressure) FROM LastIndoorTimestampRecords "
                "WHERE day = " + QString::number(QDate::currentDate().day())).toDouble();
    double pn = dbHandler->getResultFromDatabase(
                "SELECT MIN(pressure) FROM LastIndoorTimestampRecords "
                "WHERE date = " + QDate::currentDate().toString("\"dd/MM/yyyy\"")).toDouble();

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
    double stdevPx = stdevFromMeasurement("maxPressure", px);
    double stdevPn = stdevFromMeasurement("minPressure", pn);
    double stdevDeltaP = stdevFromMeasurement("(maxPressure - minPressure)", px - pn);

    double values[15] = {
        tx, tn, tx - tn,
        double(rhx), double(rhn), double(rhx - rhn),
        tdx, tdn, tdx - tdn,
        hx, hn, hx - hn,
        px, pn, px - pn
    };

    double standardDeviations[15] = {
        stdevTx, stdevTn, stdevDeltaT,
        stdevRHx, stdevRHn, stdevDeltaRH,
        stdevTdx, stdevTdn, stdevDeltaTd,
        stdevHx, stdevHn, stdevDeltaH,
        stdevPx, stdevPn, stdevDeltaP
    };

    qDebug() << "\nET";
    for (int i = 0; i < 15; i++) qDebug() << standardDeviations[i];
    qDebug() << "\nValeurs";
    for (int i = 0; i < 15; i++) qDebug() << values[i];

    double absStandardDeviations[15];
    for (int i = 0; i < 15; i++) absStandardDeviations[i] = abs(standardDeviations[i]);

    qDebug() << "\nET abs";
    for (int i = 0; i < 15; i++) qDebug() << absStandardDeviations[i];

    QStringList measurementsTranslated = {
        "la température maximale", "la température minimale", "la variation de température",
        "l'humidité maximale", "l'humidité minimale", "la variation d'humidité",
        "le point de rosée maximal", "le point de rosée minimal", "la variation de point de rosée",
        "l'humidex maximal", "l'humidex minimal", "la variation de l'humidex",
        "la pression maximale", "la pression minimale", "la variation de pression"
    };

    int indexOfMostRelevantMetric = indexOfMaxElement(absStandardDeviations);
    qDebug() << "\nIndex: " << indexOfMostRelevantMetric;

    const int decimals = (3 <= indexOfMostRelevantMetric && indexOfMostRelevantMetric < 6) ? 0 : 1;
    const QString unitWithLeadingSpace = indexOfMostRelevantMetric < 3 ? " °C" :
                                         indexOfMostRelevantMetric < 6 ? " %" :
                                         indexOfMostRelevantMetric < 9 ? " °C" :
                                         indexOfMostRelevantMetric < 12 ? "" :
                                                                         " hPa";

    return "Aujourd'hui, la valeur la plus notable est " + measurementsTranslated[indexOfMostRelevantMetric]
            + " de <b>" + locale->toString(values[indexOfMostRelevantMetric], 'f', decimals) + unitWithLeadingSpace + "</b>,<br>"
            + "ce qui correspond à un écart à la moyenne de <b>" + locale->toString(standardDeviations[indexOfMostRelevantMetric], 'f', 1)
            + "</b> " + (absStandardDeviations[indexOfMostRelevantMetric] >= 1.95 ? "écarts-types" : "écart-type") + ".";

}

int MetricsAnalyzer::indexOfMaxElement(double *array) {
    int result = 0;
    double maximum = array[0];
    for (int i = 1; i < 15; i++) {
        if (array[i] > maximum) {
            maximum = array[i];
            result = i;
        }
    }
    return result;
}
