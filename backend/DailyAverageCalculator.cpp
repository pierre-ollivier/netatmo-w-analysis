#include "DailyAverageCalculator.h"
#include "DatabaseHandler.h"
#include "types/DailyRecord.h"

double interpolateMeasurementBetweenTimestamps(
        long long targetTimestamp,
        long long timestamp1,
        long long timestamp2,
        double measurement1,
        double measurement2)
{
    double k = (targetTimestamp - timestamp1) / (timestamp2 - timestamp1);
    return measurement1 + k * (measurement2 - measurement1);
}

DailyAverageCalculator::DailyAverageCalculator(bool indoor)
{
    _indoor = indoor;
}


double DailyAverageCalculator::getFirstTemperatureFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT temperature FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp";
    return dbHandler.getResultFromDatabase(query).toDouble();
}

double DailyAverageCalculator::getLastTemperatureFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT temperature FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp desc";
    return dbHandler.getResultFromDatabase(query).toDouble();
}

int DailyAverageCalculator::getFirstHumidityFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT humidity FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp";
    return dbHandler.getResultFromDatabase(query).toInt();
}

int DailyAverageCalculator::getLastHumidityFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT humidity FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp desc";
    return dbHandler.getResultFromDatabase(query).toInt();
}

long long DailyAverageCalculator::getFirstTimestampFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT timestamp FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp";
    return dbHandler.getResultFromDatabase(query).toLongLong();
}

long long DailyAverageCalculator::getLastTimestampFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT timestamp FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp desc";
    return dbHandler.getResultFromDatabase(query).toLongLong();
}


double DailyAverageCalculator::getAverageTemperatureFromDate(QDate date) {
    double sumOfTemperatureTime = 0.;
    // To compute the average of the temperature, we compute the integral of the temperature divided by the number of seconds in the day
    // The day is split in 3 parts: before the first record, between the first and the last records, and after the last record

    DatabaseHandler dbHandler("netatmo_analysis.db");
    QDateTime dt(date, QTime(0, 0));
    long long _0hTimestamp = dt.toSecsSinceEpoch();
    long long _24hTimestamp = dt.addDays(1).toSecsSinceEpoch();
    long long firstTimestamp = getFirstTimestampFromDate(date);
    long long lastTimestamp = getLastTimestampFromDate(date);
    double firstTemperature = getFirstTemperatureFromDate(date);
    double lastTemperature = getLastTemperatureFromDate(date);

    // Between the records
    QString query = "";
    query += "SELECT SUM(sumTemp * diffTimestamp / 2) FROM (";
    query += "SELECT id, timestamp, temperature, ";
    query += "temperature + LAG(temperature) OVER (ORDER BY id) AS sumTemp, ";
    query += "timestamp - LAG(timestamp) OVER (ORDER BY id) AS diffTimestamp ";
    query += "FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\"");
    query += ")";
    sumOfTemperatureTime += dbHandler.getResultFromDatabase(query).toDouble();

    // Before the first record
    double _0hTemperature = interpolateMeasurementBetweenTimestamps(
                _0hTimestamp,
                getLastTimestampFromDate(date.addDays(-1)),
                firstTimestamp,
                getLastTemperatureFromDate(date.addDays(-1)),
                firstTemperature);
    sumOfTemperatureTime += (_0hTemperature + firstTemperature) * (firstTimestamp - _0hTimestamp) / 2;

    // After the last record
    double _24hTemperature = interpolateMeasurementBetweenTimestamps(
                _24hTimestamp,
                lastTimestamp,
                getFirstTimestampFromDate(date.addDays(1)),
                lastTemperature,
                getFirstTemperatureFromDate(date.addDays(1)));
    sumOfTemperatureTime += (_24hTemperature + lastTemperature) * (_24hTimestamp - lastTimestamp) / 2;

    // Return the result
    return sumOfTemperatureTime / (_24hTimestamp - _0hTimestamp);
}

double DailyAverageCalculator::getAverageHumidityFromDate(QDate date) {
    double sumOfHumidityTime = 0.;
    // To compute the average of the humidity, we compute the integral of the humidity divided by the number of seconds in the day
    // The day is split in 3 parts: before the first record, between the first and the last records, and after the last record

    DatabaseHandler dbHandler("netatmo_analysis.db");
    QDateTime dt(date, QTime(0, 0));
    long long _0hTimestamp = dt.toSecsSinceEpoch();
    long long _24hTimestamp = dt.addDays(1).toSecsSinceEpoch();
    long long firstTimestamp = getFirstTimestampFromDate(date);
    long long lastTimestamp = getLastTimestampFromDate(date);
    int firstHumidity = getFirstHumidityFromDate(date);
    int lastHumidity = getLastHumidityFromDate(date);

    // Between the records
    QString query = "";
    query += "SELECT SUM(sumHum * diffTimestamp / 2) FROM (";
    query += "SELECT id, timestamp, humidity, ";
    query += "humidity + LAG(humidity) OVER (ORDER BY id) AS sumHum, ";
    query += "timestamp - LAG(timestamp) OVER (ORDER BY id) AS diffTimestamp ";
    query += "FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\"");
    query += ")";
    sumOfHumidityTime += dbHandler.getResultFromDatabase(query).toInt();

    // Before the first record
    double _0hHumidity = interpolateMeasurementBetweenTimestamps(
                _0hTimestamp,
                getLastTimestampFromDate(date.addDays(-1)),
                firstTimestamp,
                getLastTemperatureFromDate(date.addDays(-1)),
                firstHumidity);
    sumOfHumidityTime += (_0hHumidity + firstHumidity) * (firstTimestamp - _0hTimestamp) / 2;

    // After the last record
    double _24hHumidity = interpolateMeasurementBetweenTimestamps(
                _24hTimestamp,
                lastTimestamp,
                getFirstTimestampFromDate(date.addDays(1)),
                lastHumidity,
                getFirstTemperatureFromDate(date.addDays(1)));
    sumOfHumidityTime += (_24hHumidity + lastHumidity) * (_24hTimestamp - lastTimestamp) / 2;

    // Return the result
    return sumOfHumidityTime / (_24hTimestamp - _0hTimestamp);
}

QString DailyAverageCalculator::indoorOrOutdoor() {
    return _indoor? "Indoor" : "Outdoor";
}
