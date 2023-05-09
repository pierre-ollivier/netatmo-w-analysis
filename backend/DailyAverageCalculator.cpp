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

DailyAverageCalculator::DailyAverageCalculator(QString pathToDatabase, bool indoor)
{
    _pathToDatabase = pathToDatabase;
    _indoor = indoor;
}

double DailyAverageCalculator::getFirstMeasurementFromDate(QDate date, QString measurementType) {
    DatabaseHandler dbHandler(_pathToDatabase);
    QString query = "SELECT " + measurementType + " FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp";
    return dbHandler.getResultFromDatabase(query).toDouble();
}

double DailyAverageCalculator::getLastMeasurementFromDate(QDate date, QString measurementType) {
    DatabaseHandler dbHandler(_pathToDatabase);
    QString query = "SELECT " + measurementType + " FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp desc";
    return dbHandler.getResultFromDatabase(query).toDouble();
}

long long DailyAverageCalculator::getFirstTimestampFromDate(QDate date) {
    DatabaseHandler dbHandler(_pathToDatabase);
    QString query = "SELECT timestamp FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp";
    return dbHandler.getResultFromDatabase(query).toLongLong();
}

long long DailyAverageCalculator::getLastTimestampFromDate(QDate date) {
    DatabaseHandler dbHandler(_pathToDatabase);
    QString query = "SELECT timestamp FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp desc";
    return dbHandler.getResultFromDatabase(query).toLongLong();
}

double DailyAverageCalculator::getAverageMeasurementFromDate(QDate date, QString measurementType) {
    double sumOfMeasurementTime = 0.;
    // To compute the average of the measurement, we compute its integral divided by the number of seconds in the day.
    // The day is split in 3 parts: before the first record, between the first and the last records, and after the last record.

    DatabaseHandler dbHandler(_pathToDatabase);
    QDateTime dt(date, QTime(0, 0));
    long long _0hTimestamp = dt.toSecsSinceEpoch();
    long long _24hTimestamp = dt.addDays(1).toSecsSinceEpoch();
    long long firstTimestamp = getFirstTimestampFromDate(date);
    long long lastTimestamp = getLastTimestampFromDate(date);
    double firstMeasurement = getFirstMeasurementFromDate(date, measurementType);
    double lastMeasurement = getLastMeasurementFromDate(date, measurementType);

    // Between the records
    QString query = "";
    query += "SELECT SUM(sumMeasurement * diffTimestamp / 2) FROM (";
    query += "SELECT id, timestamp, ";
    query += measurementType + " + LAG(" + measurementType + ") OVER (ORDER BY id) AS sumMeasurement, ";
    query += "timestamp - LAG(timestamp) OVER (ORDER BY id) AS diffTimestamp ";
    query += "FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\"");
    query += ")";
    sumOfMeasurementTime += dbHandler.getResultFromDatabase(query).toDouble();

    // Before the first record
    double _0hMeasurement = interpolateMeasurementBetweenTimestamps(
                _0hTimestamp,
                getLastTimestampFromDate(date.addDays(-1)),
                firstTimestamp,
                getLastMeasurementFromDate(date.addDays(-1), measurementType),
                firstMeasurement);
    sumOfMeasurementTime += (_0hMeasurement + firstMeasurement) * (firstTimestamp - _0hTimestamp) / 2;

    // After the last record
    double _24hMeasurement = interpolateMeasurementBetweenTimestamps(
                _24hTimestamp,
                lastTimestamp,
                getFirstTimestampFromDate(date.addDays(1)),
                lastMeasurement,
                getFirstMeasurementFromDate(date.addDays(1), measurementType));
    sumOfMeasurementTime += (_24hMeasurement + lastMeasurement) * (_24hTimestamp - lastTimestamp) / 2;

    // Return the result
    return sumOfMeasurementTime / (_24hTimestamp - _0hTimestamp);
}

QString DailyAverageCalculator::indoorOrOutdoor() {
    return _indoor? "Indoor" : "Outdoor";
}
