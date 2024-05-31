#include "MetricsAnalyzer.h"
#include <algorithm>

extern const QLocale LOCALE;
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
    locale = new QLocale(LOCALE);
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
    int maxTimestampTn = QDateTime(date, QTime(18, 0), Qt::UTC).toSecsSinceEpoch();
    int minTimestampTn = maxTimestampTn - 24 * 3600;
    int minTimestampTx = maxTimestampTn - 12 * 3600;
    int maxTimestampTx = maxTimestampTn + 12 * 3600;


    double tx = dbHandler->getResultFromDatabase(
                "SELECT max(temperature) FROM LastOutdoorTimestampRecords "
                "WHERE timestamp BETWEEN " + QString::number(minTimestampTx)
                + " AND " + QString::number(maxTimestampTx)).toDouble();
    double tn = dbHandler->getResultFromDatabase(
                "SELECT MIN(temperature) FROM LastOutdoorTimestampRecords "
                "WHERE timestamp BETWEEN " + QString::number(minTimestampTn)
                + " AND " + QString::number(maxTimestampTn)).toDouble();
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

    int month = date.month();
    QStringList measurements = {
        "maxTemperature", "minTemperature", "(maxTemperature - minTemperature)",
        "maxHumidity", "minHumidity", "(maxHumidity - minHumidity)",
        "maxDewPoint", "minDewPoint", "(maxDewPoint - minDewPoint)",
        "maxHumidex", "minHumidex", "(maxHumidex - minHumidex)",
        "maxPressure", "minPressure", "(maxPressure - minPressure)"
    };
    int highScoreBonus[15];

    for (int i = 0; i < 15; i++) {
        if (standardDeviations[i] != 0 &&
                (values[i] > mensualHigh(measurements[i], month) || values[i] < mensualLow(measurements[i], month))) {

            highScoreBonus[i] = 1;
            if (values[i] > annualHigh(measurements[i]) || values[i] < annualLow(measurements[i])) {
                highScoreBonus[i] = 2;
            }
        }
        else {
            highScoreBonus[i] = 0;
        }
    }

    double finalScores[15];
    for (int i = 0; i < 15; i++) finalScores[i] = abs(standardDeviations[i]) + highScoreBonus[i];

    QStringList measurementsTranslated = {
        "la température maximale", "la température minimale", "la variation de température",
        "l'humidité maximale", "l'humidité minimale", "la variation d'humidité",
        "le point de rosée maximal", "le point de rosée minimal", "la variation de point de rosée",
        "l'humidex maximal", "l'humidex minimal", "la variation de l'humidex",
        "la pression maximale", "la pression minimale", "la variation de pression"
    };

    int indexOfMostRelevantMetric = indexOfMaxElement(finalScores);

    QString introductoryText = currentDateIsUsed ?
                "Aujourd'hui, la valeur la plus notable est " :
                "Hier, la valeur la plus notable était ";

    QString highScoreText = highScoreBonus[indexOfMostRelevantMetric] == 2 ? "<br>C'est un nouveau record absolu." :
                            highScoreBonus[indexOfMostRelevantMetric] == 1 ? "<br>C'est un nouveau record mensuel." : "";

    QString finalText = introductoryText
            + measurementsTranslated[indexOfMostRelevantMetric] + " de <b>"
            + locale->toString(values[indexOfMostRelevantMetric], 'f', decimalsFromIndex(indexOfMostRelevantMetric))
            + unitWithLeadingSpaceFromIndex(indexOfMostRelevantMetric) + "</b>,<br>"
            + "ce qui correspond à un écart à la moyenne de <b>"
            + locale->toString(standardDeviations[indexOfMostRelevantMetric], 'f', 1)
            + "</b> " + (abs(standardDeviations[indexOfMostRelevantMetric]) >= 1.95 ? "écarts-types" : "écart-type") + "."
            + highScoreText;

    QString extraText = "";
    for (int i = 0; i < 15; i++) {
        if (i != indexOfMostRelevantMetric && finalScores[i] >= 1.95) {
            QString highScoreText = highScoreBonus[i] == 2 ? "<br>C'est un nouveau record absolu." :
                                    highScoreBonus[i] == 1 ? "<br>C'est un nouveau record mensuel." : "";
            extraText += QString("<br>")
                    + "On notera aussi "
                    + measurementsTranslated[i]
                    + " de " + locale->toString(values[i], 'f', decimalsFromIndex(i)) + unitWithLeadingSpaceFromIndex(i) + ",<br>"
                    + "ce qui correspond à un écart à la moyenne de " + locale->toString(standardDeviations[i], 'f', 1)
                    + " " + (abs(standardDeviations[i]) >= 1.95 ? "écarts-types" : "écart-type") + "."
                    + highScoreText + "<br>";
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

double MetricsAnalyzer::mensualLow(QString measurement, int month) {
    DatabaseHandler dbHandler = DatabaseHandler(PATH_TO_COPY_DATABASE);
    bool indoor = measurement.contains("pressure", Qt::CaseInsensitive);
    QString database = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return dbHandler.getResultFromDatabase(
                "SELECT min(" + measurement + ") FROM " + database + " WHERE month = " + QString::number(month)
                ).toDouble();
}

double MetricsAnalyzer::mensualHigh(QString measurement, int month) {
    DatabaseHandler dbHandler = DatabaseHandler(PATH_TO_COPY_DATABASE);
    bool indoor = measurement.contains("pressure", Qt::CaseInsensitive);
    QString database = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return dbHandler.getResultFromDatabase(
                "SELECT max(" + measurement + ") FROM " + database + " WHERE month = " + QString::number(month)
                ).toDouble();
}

double MetricsAnalyzer::annualLow(QString measurement) {
    DatabaseHandler dbHandler = DatabaseHandler(PATH_TO_COPY_DATABASE);
    bool indoor = measurement.contains("pressure", Qt::CaseInsensitive);
    QString database = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return dbHandler.getResultFromDatabase(
                "SELECT min(" + measurement + ") FROM " + database
                ).toDouble();
}

double MetricsAnalyzer::annualHigh(QString measurement) {
    DatabaseHandler dbHandler = DatabaseHandler(PATH_TO_COPY_DATABASE);
    bool indoor = measurement.contains("pressure", Qt::CaseInsensitive);
    QString database = indoor ? "IndoorDailyRecords" : "OutdoorDailyRecords";
    return dbHandler.getResultFromDatabase(
                "SELECT max(" + measurement + ") FROM " + database
                ).toDouble();
}

QPair<double, long long> MetricsAnalyzer::currentMaxTemperatureInfo() {
    DatabaseHandler dbHandler = DatabaseHandler(PATH_TO_COPY_DATABASE);
    QDateTime nowUTC = QDateTime::currentDateTimeUtc();
    if (nowUTC.time().hour() >= 6) {
        nowUTC.setTime(QTime(6, 0));
        long long minTimestamp = nowUTC.toSecsSinceEpoch();
        double maxTemperature = dbHandler.getResultFromDatabase(
                    "SELECT max(temperature) FROM LastOutdoorTimestampRecords "
                    "WHERE timestamp >= " + QString::number(minTimestamp)
                    + " ORDER BY timestamp").toDouble();
        long long maxTemperatureTimestamp = dbHandler.getResultFromDatabase(
                    "SELECT min(timestamp) FROM LastOutdoorTimestampRecords "
                    "WHERE timestamp >= " + QString::number(minTimestamp)
                    + " AND temperature = " + QString::number(maxTemperature)).toLongLong();
        return {maxTemperature, maxTemperatureTimestamp};
    }
    else {
        nowUTC.setTime(QTime(6, 0));
        long long maxTimestamp = nowUTC.toSecsSinceEpoch();
        nowUTC = nowUTC.addDays(-1);
        long long minTimestamp = nowUTC.toSecsSinceEpoch();
        double maxTemperature = dbHandler.getResultFromDatabase(
                    "SELECT max(temperature) FROM LastOutdoorTimestampRecords "
                    "WHERE timestamp BETWEEN " + QString::number(minTimestamp)
                    + " AND " + QString::number(maxTimestamp)
                    + " ORDER BY timestamp").toDouble();
        long long maxTemperatureTimestamp = dbHandler.getResultFromDatabase(
                    "SELECT min(timestamp) FROM LastOutdoorTimestampRecords "
                    "WHERE timestamp BETWEEN " + QString::number(minTimestamp)
                    + " AND " + QString::number(maxTimestamp)
                    + " AND temperature = " + QString::number(maxTemperature)).toLongLong();
        return {maxTemperature, maxTemperatureTimestamp};
    }
}

QPair<double, long long> MetricsAnalyzer::currentMinTemperatureInfo() {
    DatabaseHandler dbHandler = DatabaseHandler(PATH_TO_COPY_DATABASE);
    QDateTime nowUTC = QDateTime::currentDateTimeUtc();
    nowUTC.setTime(QTime(18, 0));
    long long maxTimestamp = nowUTC.toSecsSinceEpoch();
    nowUTC = nowUTC.addDays(-1);
    long long minTimestamp = nowUTC.toSecsSinceEpoch();
    double minTemperature = dbHandler.getResultFromDatabase(
                "SELECT min(temperature) FROM LastOutdoorTimestampRecords "
                "WHERE timestamp BETWEEN " + QString::number(minTimestamp)
                + " AND " + QString::number(maxTimestamp)
                + " ORDER BY timestamp").toDouble();
    long long minTemperatureTimestamp = dbHandler.getResultFromDatabase(
                "SELECT min(timestamp) FROM LastOutdoorTimestampRecords "
                "WHERE timestamp BETWEEN " + QString::number(minTimestamp)
                + " AND " + QString::number(maxTimestamp)
                + " AND temperature = " + QString::number(minTemperature)).toLongLong();
    return {minTemperature, minTemperatureTimestamp};
}
