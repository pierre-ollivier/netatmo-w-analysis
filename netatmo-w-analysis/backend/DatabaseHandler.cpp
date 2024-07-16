#include "DatabaseHandler.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QProgressDialog>

extern const QStringList indoorDailyRecordsParams;
extern const QStringList outdoorDailyRecordsParams;
extern const QStringList indoorTimestampsParams;
extern const QStringList outdoorTimestampsParams;

int DatabaseHandler::id = 0;

DatabaseHandler::DatabaseHandler(QString pathToDatabase) : QObject()
{
    id++;
    instance_id = id;
    _pathToDatabase = pathToDatabase;
}

DatabaseHandler::DatabaseHandler(QObject *parent, QString pathToDatabase) : QObject(parent)
{
    id++;
    instance_id = id;
    _pathToDatabase = pathToDatabase;
}

void DatabaseHandler::prepareQuery(QSqlQuery *query, QString tableName, QStringList params) {
    const int paramsSize = params.length();
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
    query->prepare(preparingQuery);
}

void DatabaseHandler::postOutdoorDailyRecord(ExtDailyRecord record, QString tableName) {
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery query(db);
        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
        }

        prepareQuery(&query, tableName, outdoorDailyRecordsParams);

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
            qDebug() << "ERROR:" << query.lastError().text();
        }
    }

    QSqlDatabase::removeDatabase(connectionName);
}

void DatabaseHandler::postOutdoorTimestampRecord(ExtTimestampRecord record, QString tableName) {
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
        }

        prepareQuery(&query, tableName, outdoorTimestampsParams);

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
            qDebug() << "ERROR:" << query.lastError().text();
        }
    }

    QSqlDatabase::removeDatabase(connectionName);
}

void DatabaseHandler::postIndoorDailyRecord(IntDailyRecord record, QString tableName) {
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
        }

        prepareQuery(&query, tableName, indoorDailyRecordsParams);

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
            qDebug() << "ERROR:" << query.lastError().text();
        }
    }

    QSqlDatabase::removeDatabase(connectionName);
}

void DatabaseHandler::postIndoorTimestampRecord(IntTimestampRecord record, QString tableName) {
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
        }

        prepareQuery(&query, tableName, indoorTimestampsParams);

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
            qDebug() << "ERROR:" << query.lastError().text();
        }
    }

    QSqlDatabase::removeDatabase(connectionName);
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
    if (path[path.size() - 1] != '/') {
        path += "/";
    }
    QDate beginDate = QDate::fromString("01/" + beginMonth, "dd/MM/yyyy");
    QDate endDate = QDate::fromString("01/" + endMonth, "dd/MM/yyyy");
    for (QDate date = beginDate; date <= endDate; date = date.addMonths(1)) {
        postFromOutdoorCsv(path + date.toString("yyyy-MM") + ".csv", tableName);
    }
}

void DatabaseHandler::postFromMultipleIndoorCsv(QString path, QString tableName, QString beginMonth, QString endMonth) {
    if (path[path.size() - 1] != '/') {
        path += "/";
    }
    QDate beginDate = QDate::fromString("01/" + beginMonth, "dd/MM/yyyy");
    QDate endDate = QDate::fromString("01/" + endMonth, "dd/MM/yyyy");
    for (QDate date = beginDate; date <= endDate; date = date.addMonths(1)) {
        postFromIndoorCsv(path + "C" + date.toString("yyyy-MM") + ".csv", tableName);
    }
}

std::vector<IntTimestampRecord> DatabaseHandler::getIntTimestampRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<IntTimestampRecord> result = std::vector<IntTimestampRecord>();
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery _query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
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
    }
    QSqlDatabase::removeDatabase(connectionName);
    return result;
}

std::vector<ExtTimestampRecord> DatabaseHandler::getExtTimestampRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<ExtTimestampRecord> result = std::vector<ExtTimestampRecord>();
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery _query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
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
    }
    QSqlDatabase::removeDatabase(connectionName);
    return result;
}

std::vector<IntDailyRecord> DatabaseHandler::getIntDailyRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<IntDailyRecord> result = std::vector<IntDailyRecord>();
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery _query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
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
    }
    QSqlDatabase::removeDatabase(connectionName);
    return result;
}

std::vector<ExtDailyRecord> DatabaseHandler::getExtDailyRecordsFromDatabase(QString query, int N) {
    if (N > 0) {
        query += " LIMIT " + QString::number(N);
    }
    std::vector<ExtDailyRecord> result = std::vector<ExtDailyRecord>();
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery _query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
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
    }
    QSqlDatabase::removeDatabase(connectionName);
    return result;
}

QVariant DatabaseHandler::getResultFromDatabase(QString query) {
    QVariant result = QVariant();
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery _query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
        }
        if (_query.exec(query)) {
            if (_query.next()) {
                result = _query.value(0);
            }
            else if (query.left(6) != "DELETE" && query.left(6) != "CREATE") {
                qDebug() << "Empty query result";
                qDebug() << query;
            }
        }
        else {
            qDebug() << "Invalid query:" << query;
        }
    }
    QSqlDatabase::removeDatabase(connectionName);
    return result;
}

std::vector<QVariant> DatabaseHandler::getResultsFromDatabase(QString query, int limit) {
    std::vector<QVariant> result = std::vector<QVariant>();
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery _query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
        }
        if (_query.exec(query)) {
            while (_query.next() && (limit == 0 || int(result.size()) < limit)) {
                result.push_back(_query.value(0));
            }
        }
        else {
            qDebug() << "Invalid query:" << query;
        }
    }
    QSqlDatabase::removeDatabase(connectionName);
    return result;
}

int DatabaseHandler::getNumberOfResultsFromDatabase(QString query) {
    int result = 0;
    QString connectionName = _pathToDatabase + "_" + QString::number(instance_id);
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(_pathToDatabase);
        QSqlQuery _query(db);

        if (!db.open()) {
            qDebug() << "Database open error. Path:" << _pathToDatabase;
        }
        if (!db.isOpen() ) {
            qDebug() << "Database is not open. Path:" << _pathToDatabase;
        }
        if (_query.exec(query)) {
            while (_query.next()) result++;
        }
        else {
            qDebug() << "Invalid query:" << query;
        }
    }
    QSqlDatabase::removeDatabase(connectionName);
    return result;
}



QDateTime DatabaseHandler::getExtremeDateTimeFromDatabase(QString tableName, QString measurement, bool asc) {
    const QString desc = asc ? "" : "desc";
    if (measurement == "") measurement = "date";
    if (tableName == "OutdoorDailyRecords" || tableName == "IndoorDailyRecords") {
        QString extremeDate = getResultFromDatabase(
                    "SELECT date from " + tableName + " "
                    "WHERE " + measurement + " IS NOT NULL "
                    "ORDER BY year " + desc + ", month " + desc + ", day " + desc + " "
                    "LIMIT 1").toString();
        return QDateTime(QDate::fromString(extremeDate, "dd/MM/yyyy"), QTime(0, 0));
    }
    else {
        QString extremeDate = getResultFromDatabase(
                    "SELECT date from " + tableName + " "
                    "WHERE " + measurement + " IS NOT NULL "
                    "ORDER BY year " + desc + ", month " + desc + ", day " + desc + " "
                    "LIMIT 1").toString();
        QString extremeTime = getResultFromDatabase(
                    "SELECT time from " + tableName + " "
                    "WHERE date = \"" + extremeDate + "\" "
                    "AND " + measurement + " IS NOT NULL "
                    "ORDER BY hour " + desc + ", minute " + desc + ", second " + desc + " "
                    "LIMIT 1").toString();
        return QDateTime(QDate::fromString(extremeDate, "dd/MM/yyyy"), QTime::fromString(extremeTime, "hh:mm:ss"));
    }
}

QDateTime DatabaseHandler::getFirstDateTimeFromDatabase(QString tableName, QString measurement) {
    return getExtremeDateTimeFromDatabase(tableName, measurement, true);
}

QDateTime DatabaseHandler::getLatestDateTimeFromDatabase(QString tableName, QString measurement) {
    return getExtremeDateTimeFromDatabase(tableName, measurement, false);
}

long long DatabaseHandler::getFirstTimestampFromDatabaseInS(QString tableName, QString measurement) {
    return getFirstDateTimeFromDatabase(tableName, measurement).toSecsSinceEpoch();
}

long long DatabaseHandler::getLatestTimestampFromDatabaseInS(QString tableName, QString measurement) {
    return getLatestDateTimeFromDatabase(tableName, measurement).toSecsSinceEpoch();
}
