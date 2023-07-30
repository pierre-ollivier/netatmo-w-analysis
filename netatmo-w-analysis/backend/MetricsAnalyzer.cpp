#include "MetricsAnalyzer.h"
#include <algorithm>
#include <QDebug>

extern const QString PATH_TO_COPY_DATABASE;

QString unitWithLeadingSpaceFromIndex(int index) {
    return index < 3 ? " °C" :
           index < 6 ? " %" :
           index < 9 ? " °C" :
           index < 12 ? "" :
                        " hPa";
}

int decimalsFromIndex(int index) {
    return (3 <= index && index < 6) ? 0 : 1;
}

MetricsAnalyzer::MetricsAnalyzer(QDate date)
{
    _date = date;
    computer = new NormalComputer(new DatabaseHandler(PATH_TO_COPY_DATABASE));
    locale = new QLocale();
}

double MetricsAnalyzer::stdevFromMeasurement(QString measurementType, double measurementValue, bool currentDateIsUsed) {
    QString table = (!measurementType.contains("pressure", Qt::CaseInsensitive)) ?
                "OutdoorDailyRecords" : "IndoorDailyRecords";
    QDate date = currentDateIsUsed ? _date : _date.addDays(-1);
    double average = computer->normalMeasurementByMovingAverage(
                table,
                date,
                measurementType,
                41);

    double stdev = computer->stdevMeasurementByMovingAverage(
                table,
                date,
                measurementType,
                average,
                41);

    return (measurementValue - average) / stdev;
}

QString MetricsAnalyzer::text(DatabaseHandler *dbHandler) {
    int hour = QDateTime::currentDateTimeUtc().time().hour();
    bool currentDateIsUsed = hour >= 14;
    QDate date = currentDateIsUsed ? QDate::currentDate() : QDate::currentDate().addDays(-1);

    double tx = dbHandler->getResultFromDatabase(
                "SELECT max(temperature) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
    double tn = dbHandler->getResultFromDatabase(
                "SELECT MIN(temperature) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
    double tdx = dbHandler->getResultFromDatabase(
                "SELECT MAX(dewPoint) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
    double tdn = dbHandler->getResultFromDatabase(
                "SELECT MIN(dewPoint) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
    double hx = dbHandler->getResultFromDatabase(
                "SELECT MAX(humidex) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
    double hn = dbHandler->getResultFromDatabase(
                "SELECT MIN(humidex) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
    int rhx = dbHandler->getResultFromDatabase(
                "SELECT MAX(humidity) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toInt();
    int rhn = dbHandler->getResultFromDatabase(
                "SELECT MIN(humidity) FROM LastOutdoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toInt();

    double px = dbHandler->getResultFromDatabase(
                "SELECT max(pressure) FROM LastIndoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();
    double pn = dbHandler->getResultFromDatabase(
                "SELECT MIN(pressure) FROM LastIndoorTimestampRecords "
                "WHERE date = " + date.toString("\"dd/MM/yyyy\"")).toDouble();

    double stdevTx = stdevFromMeasurement("maxTemperature", tx, currentDateIsUsed);
    double stdevTn = stdevFromMeasurement("minTemperature", tn, currentDateIsUsed);
    double stdevDeltaT = stdevFromMeasurement("(maxTemperature - minTemperature)", tx - tn, currentDateIsUsed);
    double stdevRHx = stdevFromMeasurement("maxHumidity", rhx, currentDateIsUsed);
    double stdevRHn = stdevFromMeasurement("minHumidity", rhn, currentDateIsUsed);
    double stdevDeltaRH = stdevFromMeasurement("(maxHumidity - minHumidity)", rhx - rhn, currentDateIsUsed);
    double stdevTdx = stdevFromMeasurement("maxDewPoint", tdx, currentDateIsUsed);
    double stdevTdn = stdevFromMeasurement("minDewPoint", tdn, currentDateIsUsed);
    double stdevDeltaTd = stdevFromMeasurement("(maxDewPoint - minDewPoint)", tdx - tdn, currentDateIsUsed);
    double stdevHx = stdevFromMeasurement("maxHumidex", hx, currentDateIsUsed);
    double stdevHn = stdevFromMeasurement("minHumidex", hn, currentDateIsUsed);
    double stdevDeltaH = stdevFromMeasurement("(maxHumidex - minHumidex)", hx - hn, currentDateIsUsed);
    double stdevPx = px != 0 ? stdevFromMeasurement("maxPressure", px, currentDateIsUsed) : 0;
    double stdevPn = pn != 0 ? stdevFromMeasurement("minPressure", pn, currentDateIsUsed) : 0;
    double stdevDeltaP = pn != 0 ? stdevFromMeasurement("(maxPressure - minPressure)", px - pn, currentDateIsUsed) : 0;

    if (currentDateIsUsed) {
        if (hour < 16) {
            stdevRHn = 0;
            stdevDeltaRH = 0;
        }
        // TODO implement other limitations on pressure, dew point and min humidex based on current measurements
        if (hour < 20) {
            stdevPx = 0;
            stdevPn = 0;
            stdevDeltaP = 0;
            stdevTdx = 0;
            stdevTdn = 0;
            stdevDeltaTd = 0;
            stdevHn = 0;
            stdevDeltaH = 0;
        }
    }

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


    QString introductoryText = currentDateIsUsed ?
                "Aujourd'hui, la valeur la plus notable est " :
                "Hier, la valeur la plus notable était ";

    QString finalText = introductoryText
            + measurementsTranslated[indexOfMostRelevantMetric]
            + " de <b>" + locale->toString(values[indexOfMostRelevantMetric], 'f', decimalsFromIndex(indexOfMostRelevantMetric))
            + unitWithLeadingSpaceFromIndex(indexOfMostRelevantMetric) + "</b>,<br>"
            + "ce qui correspond à un écart à la moyenne de <b>"
            + locale->toString(standardDeviations[indexOfMostRelevantMetric], 'f', 1)
            + "</b> " + (absStandardDeviations[indexOfMostRelevantMetric] >= 1.95 ? "écarts-types" : "écart-type") + ".";

    QString extraText = "";
    for (int i = 0; i < 15; i++) {
        if (i != indexOfMostRelevantMetric && absStandardDeviations[i] >= 1.95) {
            extraText += QString("<br>")
                    + "On notera aussi "
                    + measurementsTranslated[i]
                    + " de " + locale->toString(values[i], 'f', decimalsFromIndex(i)) + unitWithLeadingSpaceFromIndex(i) + ",<br>"
                    + "ce qui correspond à un écart à la moyenne de " + locale->toString(standardDeviations[i], 'f', 1)
                    + " " + (absStandardDeviations[i] >= 1.95 ? "écarts-types" : "écart-type") + ".<br>";
        }
    }
    return finalText + "<br>" + extraText;

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


