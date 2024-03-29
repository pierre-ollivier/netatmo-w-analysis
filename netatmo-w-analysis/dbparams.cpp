// While modifying this file, make sure to modify as well its copy in tests

#include <QStringList>

extern const QStringList indoorTimestampsParams = {
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

extern const QStringList indoorDailyRecordsParams = {
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

extern const QStringList outdoorTimestampsParams = {
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

extern const QStringList outdoorDailyRecordsParams = {
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

extern const QString lastOutdoorTimestampRecordsCreationQuery = QString()
        + "CREATE TABLE \"LastOutdoorTimestampRecords\" ("
        + "\n" + "\"id\"	INTEGER UNIQUE,"
        + "\n" + "\"timestamp\"	INTEGER,"
        + "\n" + "\"year\"	INTEGER,"
        + "\n" + "\"month\"	INTEGER,"
        + "\n" +  "\"day\"	INTEGER,"
        + "\n" + "\"date\"	TEXT,"
        + "\n" + "\"decade\"	INTEGER,"
        + "\n" + "\"weekNumber\"	INTEGER,"
        + "\n" + "\"hour\"	INTEGER,"
        + "\n" + "\"minute\"	INTEGER,"
        + "\n" + "\"second\"	INTEGER,"
        + "\n" + "\"time\"	TEXT,"
        + "\n" + "\"temperature\"	NUMERIC,"
        + "\n" + "\"humidity\"	INTEGER,"
        + "\n" + "\"dewPoint\"	REAL,"
        + "\n" + "\"humidex\"	NUMERIC,"
        + "\n" + "PRIMARY KEY(\"id\" AUTOINCREMENT)"
        + "\n" + ")";

extern const QString lastIndoorTimestampRecordsCreationQuery = QString()
        + "CREATE TABLE \"LastIndoorTimestampRecords\" ("
        + "\n" + "\"id\"	INTEGER UNIQUE,"
        + "\n" + "\"timestamp\"	INTEGER,"
        + "\n" + "\"year\"	INTEGER,"
        + "\n" + "\"month\"	INTEGER,"
        + "\n" +  "\"day\"	INTEGER,"
        + "\n" + "\"date\"	TEXT,"
        + "\n" + "\"decade\"	INTEGER,"
        + "\n" + "\"weekNumber\"	INTEGER,"
        + "\n" + "\"hour\"	INTEGER,"
        + "\n" + "\"minute\"	INTEGER,"
        + "\n" + "\"second\"	INTEGER,"
        + "\n" + "\"time\"	TEXT,"
        + "\n" + "\"temperature\"	NUMERIC,"
        + "\n" + "\"humidity\"	INTEGER,"
        + "\n" + "\"dewPoint\"	REAL,"
        + "\n" + "\"humidex\"	NUMERIC,"
        + "\n" + "\"pressure\"	NUMERIC,"
        + "\n" + "\"co2\"	INTEGER,"
        + "\n" + "\"noise\"	INTEGER,"
        + "\n" + "PRIMARY KEY(\"id\" AUTOINCREMENT)"
        + "\n" + ")";
