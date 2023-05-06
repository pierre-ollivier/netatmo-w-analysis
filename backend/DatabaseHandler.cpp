#include "DatabaseHandler.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QProgressDialog>
#include "DailyStatisticsCalculator.h"

const QString indoorTimestampsParams[18] = {
    "timestamp",
    "year",
    "month",
    "day",
    "date",
    "decade",
    "weekNumber",
    "hour",
    "minute",
    "second",
    "time",
    "temperature",
    "humidity",
    "dewPoint",
    "humidex",
    "pressure",
    "co2",
    "noise",
};

const QString indoorDailyRecordsParams[67] = {
    "year",
    "month",
    "day",
    "date",
    "decade",
    "weekNumber",
    "maxTemperature",
    "minTemperature",
    "avgTemperature",
    "maxHumidity",
    "minHumidity",
    "avgHumidity",
    "maxDewPoint",
    "minDewPoint",
    "avgDewPoint",
    "maxHumidex",
    "minHumidex",
    "avgHumidex",
    "maxPressure",
    "minPressure",
    "avgPressure",
    "maxCO2",
    "minCO2",
    "avgCO2",
    "maxNoise",
    "minNoise",
    "avgNoise",
    "maxTemperatureTimestamp",
    "maxTemperatureHour",
    "maxTemperatureMinute",
    "maxTemperatureSecond",
    "minTemperatureTimestamp",
    "minTemperatureHour",
    "minTemperatureMinute",
    "minTemperatureSecond",
    "maxHumidityTimestamp",
    "maxHumidityHour",
    "maxHumidityMinute",
    "maxHumiditySecond",
    "minHumidityTimestamp",
    "minHumidityHour",
    "minHumidityMinute",
    "minHumiditySecond",
    "maxDewPointTimestamp",
    "maxDewPointHour",
    "maxDewPointMinute",
    "maxDewPointSecond",
    "minDewPointTimestamp",
    "minDewPointHour",
    "minDewPointMinute",
    "minDewPointSecond",
    "maxHumidexTimestamp",
    "maxHumidexHour",
    "maxHumidexMinute",
    "maxHumidexSecond",
    "minHumidexTimestamp",
    "minHumidexHour",
    "minHumidexMinute",
    "minHumidexSecond",
    "maxPressureTimestamp",
    "maxPressureHour",
    "maxPressureMinute",
    "maxPressureSecond",
    "minPressureTimestamp",
    "minPressureHour",
    "minPressureMinute",
    "minPressureSecond",
};

const QString outdoorTimestampsParams[15] = {
    "timestamp",
    "year",
    "month",
    "day",
    "date",
    "decade",
    "weekNumber",
    "hour",
    "minute",
    "second",
    "time",
    "temperature",
    "humidity",
    "dewPoint",
    "humidex",
};

const QString outdoorDailyRecordsParams[50] = {
    "year",
    "month",
    "day",
    "date",
    "decade",
    "weekNumber",
    "maxTemperature",
    "minTemperature",
    "avgTemperature",
    "maxHumidity",
    "minHumidity",
    "avgHumidity",
    "maxDewPoint",
    "minDewPoint",
    "avgDewPoint",
    "maxHumidex",
    "minHumidex",
    "avgHumidex",
    "maxTemperatureTimestamp",
    "maxTemperatureHour",
    "maxTemperatureMinute",
    "maxTemperatureSecond",
    "minTemperatureTimestamp",
    "minTemperatureHour",
    "minTemperatureMinute",
    "minTemperatureSecond",
    "maxHumidityTimestamp",
    "maxHumidityHour",
    "maxHumidityMinute",
    "maxHumiditySecond",
    "minHumidityTimestamp",
    "minHumidityHour",
    "minHumidityMinute",
    "minHumiditySecond",
    "maxDewPointTimestamp",
    "maxDewPointHour",
    "maxDewPointMinute",
    "maxDewPointSecond",
    "minDewPointTimestamp",
    "minDewPointHour",
    "minDewPointMinute",
    "minDewPointSecond",
    "maxHumidexTimestamp",
    "maxHumidexHour",
    "maxHumidexMinute",
    "maxHumidexSecond",
    "minHumidexTimestamp",
    "minHumidexHour",
    "minHumidexMinute",
    "minHumidexSecond",
};

QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");

DatabaseHandler::DatabaseHandler(QString pathToDatabase)
{
    _pathToDatabase = pathToDatabase;
}

void DatabaseHandler::prepareQuery(QSqlQuery query, QString tableName, QString params[], int paramsSize) {

    // Useless for now, still some work to do to pass a QString[]
    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }

    QString preparingQuery = "INSERT INTO " + tableName + "(";
    for (int i = 0; i < paramsSize - 1; i++) {
        preparingQuery += params[i] + ",";
    }
    preparingQuery += params[paramsSize - 1];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < paramsSize - 1; i++) {
        preparingQuery += "?,";
    }
    preparingQuery += "?);";
    query.prepare(preparingQuery);
}

void DatabaseHandler::postOutdoorDailyRecord(ExtDailyRecord record, QString tableName) {
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }

    QString preparingQuery = "INSERT INTO " + tableName + "(";
    for (int i = 0; i < 49; i++) {
        preparingQuery += outdoorDailyRecordsParams[i] + ",";
    }
    preparingQuery += outdoorDailyRecordsParams[49];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 49; i++) {
        preparingQuery += "?,";
    }
    preparingQuery += "?);";


    query.prepare(preparingQuery);

//    prepareQuery(query, tableName, *outdoorDailyRecordsParams, 26);

    query.addBindValue(record.year());
    query.addBindValue(record.month());
    query.addBindValue(record.day());
    query.addBindValue(record.date().toString("dd/MM/yyyy"));
    query.addBindValue(record.decade());
    query.addBindValue(record.weekNumber());

    query.addBindValue(record.maxTemperature());
    query.addBindValue(record.minTemperature());
    query.addBindValue(record.avgTemperature());

    query.addBindValue(record.maxHumidity());
    query.addBindValue(record.minHumidity());
    query.addBindValue(record.avgHumidity());

    query.addBindValue(record.maxDewPoint());
    query.addBindValue(record.minDewPoint());
    query.addBindValue(record.avgDewPoint());

    query.addBindValue(record.maxHumidex());
    query.addBindValue(record.minHumidex());
    query.addBindValue(record.avgHumidex());

    query.addBindValue(record.maxTemperatureTimestamp());
    query.addBindValue(record.maxTemperatureTime().hour());
    query.addBindValue(record.maxTemperatureTime().minute());
    query.addBindValue(record.maxTemperatureTime().second());

    query.addBindValue(record.minTemperatureTimestamp());
    query.addBindValue(record.minTemperatureTime().hour());
    query.addBindValue(record.minTemperatureTime().minute());
    query.addBindValue(record.minTemperatureTime().second());

    query.addBindValue(record.maxHumidityTimestamp());
    query.addBindValue(record.maxHumidityTime().hour());
    query.addBindValue(record.maxHumidityTime().minute());
    query.addBindValue(record.maxHumidityTime().second());

    query.addBindValue(record.minHumidityTimestamp());
    query.addBindValue(record.minHumidityTime().hour());
    query.addBindValue(record.minHumidityTime().minute());
    query.addBindValue(record.minHumidityTime().second());

    query.addBindValue(record.maxDewPointTimestamp());
    query.addBindValue(record.maxDewPointTime().hour());
    query.addBindValue(record.maxDewPointTime().minute());
    query.addBindValue(record.maxDewPointTime().second());

    query.addBindValue(record.minDewPointTimestamp());
    query.addBindValue(record.minDewPointTime().hour());
    query.addBindValue(record.minDewPointTime().minute());
    query.addBindValue(record.minDewPointTime().second());

    query.addBindValue(record.maxHumidexTimestamp());
    query.addBindValue(record.maxHumidexTime().hour());
    query.addBindValue(record.maxHumidexTime().minute());
    query.addBindValue(record.maxHumidexTime().second());

    query.addBindValue(record.minHumidexTimestamp());
    query.addBindValue(record.minHumidexTime().hour());
    query.addBindValue(record.minHumidexTime().minute());
    query.addBindValue(record.minHumidexTime().second());

    if (!query.exec()) {
        qDebug() << "The following query could not be executed. Query: " << preparingQuery;
        qDebug() << "ERROR:" << query.lastError().text();
    }
}

void DatabaseHandler::postOutdoorTimestampRecord(ExtTimestampRecord record, QString tableName) {
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery query(db);

    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }

    QString preparingQuery = "INSERT INTO " + tableName + "(";
    for (int i = 0; i < 14; i++) {
        preparingQuery += outdoorTimestampsParams[i] + ",";
    }
    preparingQuery += outdoorTimestampsParams[14];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 14; i++) {
        preparingQuery += "?,";
    }
    preparingQuery += "?);";

    query.prepare(preparingQuery);

    query.addBindValue(record.timestamp());

    query.addBindValue(record.year());
    query.addBindValue(record.month());
    query.addBindValue(record.day());
    query.addBindValue(record.date().toString("dd/MM/yyyy"));
    query.addBindValue(record.decade());
    query.addBindValue(record.weekNumber());

    query.addBindValue(record.hour());
    query.addBindValue(record.minute());
    query.addBindValue(record.second());
    query.addBindValue(record.time().toString("hh:mm:ss"));

    query.addBindValue(record.temperature());
    query.addBindValue(record.humidity());
    query.addBindValue(record.dewPoint());
    query.addBindValue(record.humidex());

    if (!query.exec()) {
        qDebug() << "The following query could not be executed. Query: " << preparingQuery;
    }

}

void DatabaseHandler::postIndoorDailyRecord(IntDailyRecord record, QString tableName) {
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }

    QString preparingQuery = "INSERT INTO " + tableName + "(";
    for (int i = 0; i < 66; i++) {
        preparingQuery += indoorDailyRecordsParams[i] + ",";
    }
    preparingQuery += indoorDailyRecordsParams[66];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 66; i++) {
        preparingQuery += "?,";
    }
    preparingQuery += "?);";


    query.prepare(preparingQuery);

//    prepareQuery(query, tableName, *outdoorDailyRecordsParams, 26);

    query.addBindValue(record.year());
    query.addBindValue(record.month());
    query.addBindValue(record.day());
    query.addBindValue(record.date().toString("dd/MM/yyyy"));
    query.addBindValue(record.decade());
    query.addBindValue(record.weekNumber());

    query.addBindValue(record.maxTemperature());
    query.addBindValue(record.minTemperature());
    query.addBindValue(record.avgTemperature());

    query.addBindValue(record.maxHumidity());
    query.addBindValue(record.minHumidity());
    query.addBindValue(record.avgHumidity());

    query.addBindValue(record.maxDewPoint());
    query.addBindValue(record.minDewPoint());
    query.addBindValue(record.avgDewPoint());

    query.addBindValue(record.maxHumidex());
    query.addBindValue(record.minHumidex());
    query.addBindValue(record.avgHumidex());

    query.addBindValue(record.maxPressure());
    query.addBindValue(record.minPressure());
    query.addBindValue(record.avgPressure());

    query.addBindValue(record.maxCO2());
    query.addBindValue(record.minCO2());
    query.addBindValue(record.avgCO2());

    query.addBindValue(record.maxNoise());
    query.addBindValue(record.minNoise());
    query.addBindValue(record.avgNoise());

    query.addBindValue(record.maxTemperatureTimestamp());
    query.addBindValue(record.maxTemperatureTime().hour());
    query.addBindValue(record.maxTemperatureTime().minute());
    query.addBindValue(record.maxTemperatureTime().second());

    query.addBindValue(record.minTemperatureTimestamp());
    query.addBindValue(record.minTemperatureTime().hour());
    query.addBindValue(record.minTemperatureTime().minute());
    query.addBindValue(record.minTemperatureTime().second());

    query.addBindValue(record.maxHumidityTimestamp());
    query.addBindValue(record.maxHumidityTime().hour());
    query.addBindValue(record.maxHumidityTime().minute());
    query.addBindValue(record.maxHumidityTime().second());

    query.addBindValue(record.minHumidityTimestamp());
    query.addBindValue(record.minHumidityTime().hour());
    query.addBindValue(record.minHumidityTime().minute());
    query.addBindValue(record.minHumidityTime().second());

    query.addBindValue(record.maxDewPointTimestamp());
    query.addBindValue(record.maxDewPointTime().hour());
    query.addBindValue(record.maxDewPointTime().minute());
    query.addBindValue(record.maxDewPointTime().second());

    query.addBindValue(record.minDewPointTimestamp());
    query.addBindValue(record.minDewPointTime().hour());
    query.addBindValue(record.minDewPointTime().minute());
    query.addBindValue(record.minDewPointTime().second());

    query.addBindValue(record.maxHumidexTimestamp());
    query.addBindValue(record.maxHumidexTime().hour());
    query.addBindValue(record.maxHumidexTime().minute());
    query.addBindValue(record.maxHumidexTime().second());

    query.addBindValue(record.minHumidexTimestamp());
    query.addBindValue(record.minHumidexTime().hour());
    query.addBindValue(record.minHumidexTime().minute());
    query.addBindValue(record.minHumidexTime().second());

    query.addBindValue(record.maxPressureTimestamp());
    query.addBindValue(record.maxPressureTime().hour());
    query.addBindValue(record.maxPressureTime().minute());
    query.addBindValue(record.maxPressureTime().second());

    query.addBindValue(record.minPressureTimestamp());
    query.addBindValue(record.minPressureTime().hour());
    query.addBindValue(record.minPressureTime().minute());
    query.addBindValue(record.minPressureTime().second());

    if (!query.exec()) {
        qDebug() << "The following query could not be executed. Query: " << preparingQuery;
    }
}

void DatabaseHandler::postIndoorTimestampRecord(IntTimestampRecord record, QString tableName) {
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery query(db);

    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }

    QString preparingQuery = "INSERT INTO " + tableName + "(";
    for (int i = 0; i < 17; i++) {
        preparingQuery += indoorTimestampsParams[i] + ",";
    }
    preparingQuery += indoorTimestampsParams[17];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 17; i++) {
        preparingQuery += "?,";
    }
    preparingQuery += "?);";

    query.prepare(preparingQuery);

    query.addBindValue(record.timestamp());

    query.addBindValue(record.year());
    query.addBindValue(record.month());
    query.addBindValue(record.day());
    query.addBindValue(record.date().toString("dd/MM/yyyy"));
    query.addBindValue(record.decade());
    query.addBindValue(record.weekNumber());

    query.addBindValue(record.hour());
    query.addBindValue(record.minute());
    query.addBindValue(record.second());
    query.addBindValue(record.time().toString("hh:mm:ss"));

    query.addBindValue(record.temperature());
    query.addBindValue(record.humidity());
    query.addBindValue(record.dewPoint());
    query.addBindValue(record.humidex());
    query.addBindValue(record.pressure());
    query.addBindValue(record.co2());
    query.addBindValue(record.noise());

    if (!query.exec()) {
        qDebug() << "The following query could not be executed. Query: " << preparingQuery;
    }
}

void DatabaseHandler::postFromOutdoorCsv(QString pathToCsv, QString tableName, QDate beginDate, QDate endDate) {

    // Set default values of beginDate and endDate according to the file name
    if (beginDate.isNull()) {
        const int lenOfPathToCsv = pathToCsv.size() - 4;  // don't take into account .csv at the end
        int month = pathToCsv.mid(lenOfPathToCsv - 2, 2).toInt();
        int year = pathToCsv.mid(lenOfPathToCsv - 7, 4).toInt();
        beginDate.setDate(year, month, 1);  // first day of the month
        endDate.setDate(year, month, 1);
        endDate.setDate(year, month, endDate.daysInMonth());  // last day of the month
    }

    QFile file(pathToCsv);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream textStream(&file);
        const int minTimestamp = beginDate.startOfDay().toSecsSinceEpoch();
        const int maxTimestamp = endDate.endOfDay().toSecsSinceEpoch();
        QProgressDialog progress("Ajout des nouvelles données...", "Annuler", minTimestamp, maxTimestamp);
        progress.setWindowModality(Qt::WindowModal);
        progress.setValue(minTimestamp);

        while(!textStream.atEnd()) {
            if (progress.wasCanceled()) {
                break;
            }
            QString s = textStream.readLine();
            if (QDate::currentDate().year() >= 2030) {
                qDebug() << "WARNING: the function postFromOutdoorCsv will stop working on May 18th, 2033 due to a timestamp issue";
            }
            if (s[0] != '1') {  // if s doesn't start with '1', it is not a timestamp (until 2033) and we ignore it
                continue;
            }
            QStringList l = s.split(';');
            long long timestamp = l.at(0).toLongLong();
            double t = l.at(2).toDouble();
            int rh = l.at(3).toInt();

            // Check that timestamp is within the range of [beginDate, endDate]
            QDateTime dateAssociatedToTimestamp = QDateTime();
            dateAssociatedToTimestamp.setSecsSinceEpoch(timestamp);

            if (beginDate <= dateAssociatedToTimestamp.date() && dateAssociatedToTimestamp.date() <= endDate) {
                ExtTimestampRecord record(timestamp, t, rh);
                postOutdoorTimestampRecord(record, tableName);
                progress.setValue(timestamp);
            }
        }
        progress.setValue(maxTimestamp);
    }
}

void DatabaseHandler::postFromIndoorCsv(QString pathToCsv, QString tableName, QDate beginDate, QDate endDate) {

    // Set default values of beginDate and endDate according to the file name
    if (beginDate.isNull()) {
        const int lenOfPathToCsv = pathToCsv.size() - 4;  // don't take into account .csv at the end
        int month = pathToCsv.mid(lenOfPathToCsv - 2, 2).toInt();
        int year = pathToCsv.mid(lenOfPathToCsv - 7, 4).toInt();
        beginDate.setDate(year, month, 1);  // first day of the month
        endDate.setDate(year, month, 1);
        endDate.setDate(year, month, endDate.daysInMonth());  // last day of the month
    }

    QFile file(pathToCsv);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream textStream(&file);
        const int minTimestamp = beginDate.startOfDay().toSecsSinceEpoch();
        const int maxTimestamp = endDate.endOfDay().toSecsSinceEpoch();
        QProgressDialog progress("Ajout des nouvelles données...", "Annuler", minTimestamp, maxTimestamp);
        progress.setWindowModality(Qt::WindowModal);
        progress.setValue(minTimestamp);

        while(!textStream.atEnd()) {
            if (progress.wasCanceled()) {
                break;
            }
            QString s = textStream.readLine();
            if (QDate::currentDate().year() >= 2030) {
                qDebug() << "WARNING: the function postFromOutdoorCsv will stop working on May 18th, 2033 due to a timestamp issue";
            }
            if (s[0] != '1') {  // if s doesn't start with '1', it is not a timestamp (until 2033) and we ignore it
                continue;
            }
            QStringList l = s.split(';');
            long long timestamp = l.at(0).toLongLong();
            double t = l.at(2).toDouble();
            int rh = l.at(3).toInt();
            int co2 = l.at(4).toInt();
            int noise = l.at(5).toInt();
            double pressure = l.at(6).toDouble();

            // Check that timestamp is within the range of [beginDate, endDate]
            QDateTime dateAssociatedToTimestamp = QDateTime();
            dateAssociatedToTimestamp.setSecsSinceEpoch(timestamp);

            if (beginDate <= dateAssociatedToTimestamp.date() && dateAssociatedToTimestamp.date() <= endDate) {
                IntTimestampRecord record(timestamp, t, rh, pressure, co2, noise);
                postIndoorTimestampRecord(record, tableName);
                progress.setValue(timestamp);
            }
        }
        progress.setValue(maxTimestamp);
    }
}

void DatabaseHandler::postFromMultipleOutdoorCsv(QString path, QString tableName, QString beginMonth, QString endMonth) {
    // TODO: make this function and the following one more robust to paths ending or not with /
    QDate beginDate = QDate::fromString("01/" + beginMonth, "dd/MM/yyyy");
    QDate endDate = QDate::fromString("01/" + endMonth, "dd/MM/yyyy");
    for (QDate date = beginDate; date <= endDate; date = date.addMonths(1)) {
        postFromOutdoorCsv(path + "/" + date.toString("yyyy-MM") + ".csv", tableName);
    }
}

void DatabaseHandler::postFromMultipleIndoorCsv(QString path, QString tableName, QString beginMonth, QString endMonth) {
    QDate beginDate = QDate::fromString("01/" + beginMonth, "dd/MM/yyyy");
    QDate endDate = QDate::fromString("01/" + endMonth, "dd/MM/yyyy");
    for (QDate date = beginDate; date <= endDate; date = date.addMonths(1)) {
        postFromIndoorCsv(path + "/C" + date.toString("yyyy-MM") + ".csv", tableName);
    }
}

std::vector<IntTimestampRecord> DatabaseHandler::getIntTimestampRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<IntTimestampRecord> result = std::vector<IntTimestampRecord>();
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery _query(db);

    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }
    if (_query.exec(query)) {
        while (_query.next()) {
            long long timestamp = _query.value(1).toLongLong();
            double temperature = _query.value(12).toDouble();
            int humidity = _query.value(13).toInt();
            double pressure = _query.value(17).toDouble();
            int co2 = _query.value(18).toInt();
            int noise = _query.value(19).toInt();
            result.push_back(
                        IntTimestampRecord(
                            timestamp,
                            temperature,
                            humidity,
                            pressure,
                            co2,
                            noise)
                        );
        }
    }
    return result;
}

std::vector<ExtTimestampRecord> DatabaseHandler::getExtTimestampRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<ExtTimestampRecord> result = std::vector<ExtTimestampRecord>();
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery _query(db);

    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }
    if (_query.exec(query)) {
        while (_query.next()) {
            long long timestamp = _query.value(1).toLongLong();
            double temperature = _query.value(12).toDouble();
            int humidity = _query.value(13).toInt();
            result.push_back(
                        ExtTimestampRecord(
                            timestamp,
                            temperature,
                            humidity)
                        );
        }
    }
    return result;
}

std::vector<IntDailyRecord> DatabaseHandler::getIntDailyRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<IntDailyRecord> result = std::vector<IntDailyRecord>();
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery _query(db);

    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }
    if (_query.exec(query)) {
        while (_query.next()) {
            int year = _query.value(1).toInt();
            int month = _query.value(2).toInt();
            int day = _query.value(3).toInt();
            double maxTemperature = _query.value(7).toDouble();
            double minTemperature = _query.value(8).toDouble();
            double avgTemperature = _query.value(9).toDouble();
            int maxHumidity = _query.value(10).toInt();
            int minHumidity = _query.value(11).toInt();
            double avgHumidity = _query.value(12).toDouble();
            double maxDewPoint = _query.value(13).toDouble();
            double minDewPoint = _query.value(14).toDouble();
            double avgDewPoint = _query.value(15).toDouble();
            int maxHumidex = _query.value(16).toInt();
            int minHumidex = _query.value(17).toInt();
            double avgHumidex = _query.value(18).toDouble();
            double maxPressure = _query.value(19).toDouble();
            double minPressure = _query.value(20).toDouble();
            double avgPressure = _query.value(21).toDouble();
            int maxCO2 = _query.value(22).toInt();
            int minCO2 = _query.value(23).toInt();
            double avgCO2 = _query.value(24).toDouble();
            int maxNoise = _query.value(25).toInt();
            int minNoise = _query.value(26).toInt();
            double avgNoise = _query.value(27).toDouble();
            long long maxTemperatureTimestamp = _query.value(28).toLongLong();
            long long minTemperatureTimestamp = _query.value(32).toLongLong();
            long long maxHumidityTimestamp = _query.value(36).toLongLong();
            long long minHumidityTimestamp = _query.value(40).toLongLong();
            long long maxDewPointTimestamp = _query.value(44).toLongLong();
            long long minDewPointTimestamp = _query.value(48).toLongLong();
            long long maxHumidexTimestamp = _query.value(52).toLongLong();
            long long minHumidexTimestamp = _query.value(56).toLongLong();
            long long maxPressureTimestamp = _query.value(60).toLongLong();
            long long minPressureTimestamp = _query.value(64).toLongLong();
            result.push_back(
                        IntDailyRecord(
                            QDate(year, month, day),
                            maxTemperature,
                            minTemperature,
                            avgTemperature,
                            maxHumidity,
                            minHumidity,
                            avgHumidity,
                            maxDewPoint,
                            minDewPoint,
                            avgDewPoint,
                            maxHumidex,
                            minHumidex,
                            avgHumidex,
                            maxPressure,
                            minPressure,
                            avgPressure,
                            maxCO2,
                            minCO2,
                            avgCO2,
                            maxNoise,
                            minNoise,
                            avgNoise,
                            maxTemperatureTimestamp,
                            minTemperatureTimestamp,
                            maxHumidityTimestamp,
                            minHumidityTimestamp,
                            maxDewPointTimestamp,
                            minDewPointTimestamp,
                            maxHumidexTimestamp,
                            minHumidexTimestamp,
                            maxPressureTimestamp,
                            minPressureTimestamp)
                        );
        }
    }
    return result;
}

std::vector<ExtDailyRecord> DatabaseHandler::getExtDailyRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<ExtDailyRecord> result = std::vector<ExtDailyRecord>();
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery _query(db);

    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }
    if (_query.exec(query)) {
        while (_query.next()) {
            int year = _query.value(1).toInt();
            int month = _query.value(2).toInt();
            int day = _query.value(3).toInt();
            double maxTemperature = _query.value(7).toDouble();
            double minTemperature = _query.value(8).toDouble();
            double avgTemperature = _query.value(9).toDouble();
            int maxHumidity = _query.value(10).toInt();
            int minHumidity = _query.value(11).toInt();
            double avgHumidity = _query.value(12).toDouble();
            double maxDewPoint = _query.value(13).toDouble();
            double minDewPoint = _query.value(14).toDouble();
            double avgDewPoint = _query.value(15).toDouble();
            double maxHumidex = _query.value(16).toDouble();
            double minHumidex = _query.value(17).toDouble();
            double avgHumidex = _query.value(18).toDouble();
            long long maxTemperatureTimestamp = _query.value(19).toLongLong();
            long long minTemperatureTimestamp = _query.value(23).toLongLong();
            long long maxHumidityTimestamp = _query.value(27).toLongLong();
            long long minHumidityTimestamp = _query.value(31).toLongLong();
            long long maxDewPointTimestamp = _query.value(35).toLongLong();
            long long minDewPointTimestamp = _query.value(39).toLongLong();
            long long maxHumidexTimestamp = _query.value(43).toLongLong();
            long long minHumidexTimestamp = _query.value(47).toLongLong();
            result.push_back(
                        ExtDailyRecord(
                            QDate(year, month, day),
                            maxTemperature,
                            minTemperature,
                            avgTemperature,
                            maxHumidity,
                            minHumidity,
                            avgHumidity,
                            maxDewPoint,
                            minDewPoint,
                            avgDewPoint,
                            maxHumidex,
                            minHumidex,
                            avgHumidex,
                            maxTemperatureTimestamp,
                            minTemperatureTimestamp,
                            maxHumidityTimestamp,
                            minHumidityTimestamp,
                            maxDewPointTimestamp,
                            minDewPointTimestamp,
                            maxHumidexTimestamp,
                            minHumidexTimestamp)
                        );
        }
    }
    return result;
}

QVariant DatabaseHandler::getResultFromDatabase(QString query) {
    std::vector<ExtDailyRecord> result = std::vector<ExtDailyRecord>();
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    QSqlQuery _query(db);

    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }
    if (_query.exec(query)) {
        if (_query.next()) {
            return _query.value(0);
        }
        else {
            qDebug() << "Empty query result";
            qDebug() << query;
        }
    }
    else {
        qDebug() << "Invalid query:" << query;
    }
    return QVariant();
}

void DatabaseHandler::updateOutdoorDailyRecords(QDate beginDate, QDate endDate, bool verbose) {
    DailyStatisticsCalculator dailyCalculator = DailyStatisticsCalculator();
    QProgressDialog progress("Ajout des nouvelles données...", "Annuler", beginDate.toJulianDay(), endDate.toJulianDay());
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(beginDate.toJulianDay());
    if (!verbose) progress.reset();
    for (QDate date = beginDate; date <= endDate; date = date.addDays(1)) {
        ExtDailyRecord record(
                    date,
                    dailyCalculator.getMaxTemperatureFromDate(date),
                    dailyCalculator.getMinTemperatureFromDate(date),
                    dailyCalculator.getAvgTemperatureFromDate(date),
                    dailyCalculator.getMaxHumidityFromDate(date),
                    dailyCalculator.getMinHumidityFromDate(date),
                    dailyCalculator.getAvgHumidityFromDate(date),
                    dailyCalculator.getMaxDewPointFromDate(date),
                    dailyCalculator.getMinDewPointFromDate(date),
                    dailyCalculator.getAvgDewPointFromDate(date),
                    dailyCalculator.getMaxHumidexFromDate(date),
                    dailyCalculator.getMinHumidexFromDate(date),
                    dailyCalculator.getAvgHumidexFromDate(date),
                    dailyCalculator.getMaxTemperatureTimestampFromDate(date),
                    dailyCalculator.getMinTemperatureTimestampFromDate(date),
                    dailyCalculator.getMaxHumidityTimestampFromDate(date),
                    dailyCalculator.getMinHumidityTimestampFromDate(date),
                    dailyCalculator.getMaxDewPointTimestampFromDate(date),
                    dailyCalculator.getMinDewPointTimestampFromDate(date),
                    dailyCalculator.getMaxHumidexTimestampFromDate(date),
                    dailyCalculator.getMinHumidexTimestampFromDate(date)
                    );
        postOutdoorDailyRecord(record, "OutdoorDailyRecords");
        if (verbose) progress.setValue(date.toJulianDay());
    }
}

void DatabaseHandler::updateIndoorDailyRecords(QDate beginDate, QDate endDate, bool verbose) {
    DailyStatisticsCalculator dailyCalculator = DailyStatisticsCalculator();
    QProgressDialog progress("Ajout des nouvelles données...", "Annuler", beginDate.toJulianDay(), endDate.toJulianDay());
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(beginDate.toJulianDay());
    if (!verbose) progress.reset();
    for (QDate date = beginDate; date <= endDate; date = date.addDays(1)) {
        IntDailyRecord record(
                    date,
                    dailyCalculator.getMaxTemperatureFromDate(date, true),
                    dailyCalculator.getMinTemperatureFromDate(date, true),
                    dailyCalculator.getAvgTemperatureFromDate(date, true),
                    dailyCalculator.getMaxHumidityFromDate(date, true),
                    dailyCalculator.getMinHumidityFromDate(date, true),
                    dailyCalculator.getAvgHumidityFromDate(date, true),
                    dailyCalculator.getMaxDewPointFromDate(date, true),
                    dailyCalculator.getMinDewPointFromDate(date, true),
                    dailyCalculator.getAvgDewPointFromDate(date, true),
                    dailyCalculator.getMaxHumidexFromDate(date, true),
                    dailyCalculator.getMinHumidexFromDate(date, true),
                    dailyCalculator.getAvgHumidexFromDate(date, true),
                    dailyCalculator.getMaxPressureFromDate(date),
                    dailyCalculator.getMinPressureFromDate(date),
                    dailyCalculator.getAvgPressureFromDate(date),
                    dailyCalculator.getMaxCO2FromDate(date),
                    dailyCalculator.getMinCO2FromDate(date),
                    dailyCalculator.getAvgCO2FromDate(date),
                    dailyCalculator.getMaxNoiseFromDate(date),
                    dailyCalculator.getMinNoiseFromDate(date),
                    dailyCalculator.getAvgCO2FromDate(date),
                    dailyCalculator.getMaxTemperatureTimestampFromDate(date, true),
                    dailyCalculator.getMinTemperatureTimestampFromDate(date, true),
                    dailyCalculator.getMaxHumidityTimestampFromDate(date, true),
                    dailyCalculator.getMinHumidityTimestampFromDate(date, true),
                    dailyCalculator.getMaxDewPointTimestampFromDate(date, true),
                    dailyCalculator.getMinDewPointTimestampFromDate(date, true),
                    dailyCalculator.getMaxHumidexTimestampFromDate(date, true),
                    dailyCalculator.getMinHumidexTimestampFromDate(date, true),
                    dailyCalculator.getMaxPressureTimestampFromDate(date),
                    dailyCalculator.getMinPressureTimestampFromDate(date)
                    );
        postIndoorDailyRecord(record, "IndoorDailyRecords");
        if (verbose) progress.setValue(date.toJulianDay());
    }
}
