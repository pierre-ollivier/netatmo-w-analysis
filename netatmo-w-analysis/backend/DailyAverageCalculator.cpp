#include "DailyAverageCalculator.h"
#include "../netatmo-w-analysis/types/DailyRecord.h"

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
    dbHandler = new DatabaseHandler(this, _pathToDatabase);
}

DailyAverageCalculator::DailyAverageCalculator(QObject *parent, QString pathToDatabase, bool indoor) : QObject(parent)
{
    _pathToDatabase = pathToDatabase;
    _indoor = indoor;
    dbHandler = new DatabaseHandler(this, _pathToDatabase);
}


double DailyAverageCalculator::getFirstMeasurementFromDate(QDate date, QString measurementType) {
    QString query = "SELECT " + measurementType + " FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp";
    return dbHandler->getResultFromDatabase(query).toDouble();
}

double DailyAverageCalculator::getLastMeasurementFromDate(QDate date, QString measurementType) {
    QString query = "SELECT " + measurementType + " FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp desc";
    return dbHandler->getResultFromDatabase(query).toDouble();
}

long long DailyAverageCalculator::getFirstTimestampFromDate(QDate date) {
    QString query = "SELECT timestamp FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp";
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

long long DailyAverageCalculator::getLastTimestampFromDate(QDate date) {
    QString query = "SELECT timestamp FROM " + indoorOrOutdoor() + "TimestampRecords ";
    query += "WHERE date = " + date.toString("\"dd/MM/yyyy\" ");
    query += "ORDER BY timestamp desc";
    return dbHandler->getResultFromDatabase(query).toLongLong();
}

QList<ExtTimestampRecord> DailyAverageCalculator::extTimestampRecordsForDateWithAdjacents(QDate date, QList<ExtTimestampRecord> records) {
    QList<ExtTimestampRecord> result = QList<ExtTimestampRecord>();
    if (records.size() == 0) return result;
    ExtTimestampRecord lastRecordBeforeDate = records[0];
    ExtTimestampRecord firstRecordAfterDate = records[0];

    for (ExtTimestampRecord record : records) {
        if (record.date() == date) result.append(record);
        else if (record.date() < date && record.timestamp() > lastRecordBeforeDate.timestamp()) lastRecordBeforeDate = record;
        else if (record.date() > date && record.timestamp() < firstRecordAfterDate.timestamp()) firstRecordAfterDate = record;
    }
    result.append(lastRecordBeforeDate);
    result.append(firstRecordAfterDate);
    std::sort(result.begin(), result.end(), [](ExtTimestampRecord &x, ExtTimestampRecord &y){ return x.timestamp() < y.timestamp(); });
    return result;
}

double DailyAverageCalculator::getAverageMeasurementFromDate(QDate date, QString measurementType) {
    double sumOfMeasurementTime = 0.;
    // To compute the average of the measurement, we compute its integral divided by the number of seconds in the day.
    // The day is split in 3 parts: before the first record, between the first and the last records, and after the last record.

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
    sumOfMeasurementTime += dbHandler->getResultFromDatabase(query).toDouble();

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

double DailyAverageCalculator::getOutdoorAverageMeasurementFromDate(QDate date, QList<ExtTimestampRecord> records, QString measurementType) {
    double sumOfMeasurementTime = 0.;
    // To compute the average of the measurement, we compute its integral divided by the number of seconds in the day.
    // The day is split in 3 parts: before the first record, between the first and the last records, and after the last record.
    QList<ExtTimestampRecord> selectedSortedRecords = extTimestampRecordsForDateWithAdjacents(date, records);

    QDateTime dt(date, QTime(0, 0));

    long long _0hTimestamp = dt.toSecsSinceEpoch();
    long long _24hTimestamp = dt.addDays(1).toSecsSinceEpoch();
    long long firstTimestamp = selectedSortedRecords[1].timestamp();
    long long lastTimestamp = selectedSortedRecords[selectedSortedRecords.size() - 2].timestamp();
    double firstMeasurement = selectedSortedRecords[0].measurement(measurementType).toDouble();
    double lastMeasurement = selectedSortedRecords[selectedSortedRecords.size() - 2].measurement(measurementType).toDouble();

    // Between the records
    for (int i = 1; i <= selectedSortedRecords.size() - 3; i++) {
        sumOfMeasurementTime += (
                                    selectedSortedRecords[i].measurement(measurementType).toDouble()
                                    + selectedSortedRecords[i + 1].measurement(measurementType).toDouble()
                                 ) * (selectedSortedRecords[i + 1].timestamp() - selectedSortedRecords[i].timestamp()) / 2;
    }

    // Before the first record
    double _0hMeasurement = interpolateMeasurementBetweenTimestamps(
        _0hTimestamp,
        selectedSortedRecords[0].timestamp(),
        firstTimestamp,
        selectedSortedRecords[0].measurement(measurementType).toDouble(),
        firstMeasurement);
    sumOfMeasurementTime += (_0hMeasurement + firstMeasurement) * (firstTimestamp - _0hTimestamp) / 2;

    // After the last record
    double _24hMeasurement = interpolateMeasurementBetweenTimestamps(
        _24hTimestamp,
        lastTimestamp,
        selectedSortedRecords[selectedSortedRecords.size() - 1].timestamp(),
        lastMeasurement,
        selectedSortedRecords[selectedSortedRecords.size() - 1].measurement(measurementType).toDouble());
    sumOfMeasurementTime += (_24hMeasurement + lastMeasurement) * (_24hTimestamp - lastTimestamp) / 2;

    // Return the result
    return sumOfMeasurementTime / (_24hTimestamp - _0hTimestamp);
}

QString DailyAverageCalculator::indoorOrOutdoor() {
    return _indoor? "Indoor" : "Outdoor";
}
