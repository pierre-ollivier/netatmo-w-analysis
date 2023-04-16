#include "DatabaseHandler.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

const QString outdoorDailyRecordsParams[26] = {
    "year",
    "month",
    "day",
    "date",
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

const QString outdoorDailyTimestampsParams[13] = {
    "timestamp",
    "year",
    "month",
    "day",
    "date",
    "hour",
    "minute",
    "second",
    "time",
    "temperature",
    "humidity",
    "dewPoint",
    "humidex",
};

QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");

DatabaseHandler::DatabaseHandler(QString pathToDatabase)
{
    _pathToDatabase = pathToDatabase;
}

void DatabaseHandler::postOutdoorDailyRecord(DailyRecord record, QString tableName) {
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }

    QString preparingQuery = "INSERT INTO " + tableName + "(";
    for (int i = 0; i < 25; i++) {
        preparingQuery += outdoorDailyRecordsParams[i] + ",";
    }
    preparingQuery += outdoorDailyRecordsParams[25];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 25; i++) {
        preparingQuery += "?,";
    }
    preparingQuery += "?);";

    QSqlQuery query(db);
    query.prepare(preparingQuery);

    query.addBindValue(record.year());
    query.addBindValue(record.month());
    query.addBindValue(record.day());
    query.addBindValue(record.date().toString("dd/MM/yyyy"));

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

void DatabaseHandler::postOutdoorTimestampRecord(TimestampRecord record, QString tableName) {
    db.setDatabaseName("../netatmo-w-analysis/" + _pathToDatabase);
    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }

    QString preparingQuery = "INSERT INTO " + tableName + "(";
    for (int i = 0; i < 12; i++) {
        preparingQuery += outdoorDailyTimestampsParams[i] + ",";
    }
    preparingQuery += outdoorDailyTimestampsParams[12];
    preparingQuery += ") VALUES (";
    for (int i = 0; i < 12; i++) {
        preparingQuery += "?,";
    }
    preparingQuery += "?);";

    QSqlQuery query(db);
    query.prepare(preparingQuery);

    query.addBindValue(record.timestamp());

    query.addBindValue(record.date().year());
    query.addBindValue(record.date().month());
    query.addBindValue(record.date().day());
    query.addBindValue(record.date().toString("dd/MM/yyyy"));

    query.addBindValue(record.time().hour());
    query.addBindValue(record.time().minute());
    query.addBindValue(record.time().second());
    query.addBindValue(record.time().toString("hh:mm:ss"));

    query.addBindValue(record.temperature());
    query.addBindValue(record.humidity());
    query.addBindValue(record.dewPoint());
    query.addBindValue(record.humidex());

    if (!query.exec()) {
        qDebug() << "The following query could not be executed. Query: " << preparingQuery;
    }

}
