#include "DatabaseHandler.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QProgressDialog>

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

const QString indoorDailyRecordsParams[39] = {
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
    "maxPressure",
    "minPressure",
    "avgPressure",
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

const QString outdoorDailyRecordsParams[28] = {
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
    for (int i = 0; i < 27; i++) {
        preparingQuery += outdoorDailyRecordsParams[i] + ",";
    }
    preparingQuery += outdoorDailyRecordsParams[27];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 27; i++) {
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

    if (!query.exec()) {
        qDebug() << "The following query could not be executed. Query: " << preparingQuery;
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
    for (int i = 0; i < 38; i++) {
        preparingQuery += outdoorDailyRecordsParams[i] + ",";
    }
    preparingQuery += indoorDailyRecordsParams[38];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 38; i++) {
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
