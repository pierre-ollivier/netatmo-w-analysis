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

DatabaseHandler::DatabaseHandler(QString pathToDatabase)
{
    _pathToDatabase = pathToDatabase;
}

void DatabaseHandler::postDailyRecord(DailyRecord record, QString tableName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
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
    for (int i = 0; i< 25; i++) {
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
