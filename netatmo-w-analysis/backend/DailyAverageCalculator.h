#ifndef DAILYAVERAGECALCULATOR_H
#define DAILYAVERAGECALCULATOR_H

#include <QDate>
#include <QList>
#include <QObject>

#include "../netatmo-w-analysis/backend/DatabaseHandler.h"
#include "../netatmo-w-analysis/types/ExtTimestampRecord.h"
#include "../netatmo-w-analysis/types/IntDailyRecord.h"

class DailyAverageCalculator : public QObject
{
public:
    DailyAverageCalculator(QString pathToDatabase, bool indoor);
    DailyAverageCalculator(QObject *parent, QString pathToDatabase, bool indoor);
    double getAverageMeasurementFromDate(QDate date, QString measurementType);
    double getIndoorAverageMeasurementFromDate(QDate date, QList<IntTimestampRecord> records, QString measurementType);
    double getOutdoorAverageMeasurementFromDate(QDate date, QList<ExtTimestampRecord> records, QString measurementType);

    double getFirstMeasurementFromDate(QDate date, QString measurementType);
    double getLastMeasurementFromDate(QDate date, QString measurementType);
    long long getFirstTimestampFromDate(QDate date);
    long long getLastTimestampFromDate(QDate date);

    QString indoorOrOutdoor();

    QList<ExtTimestampRecord> extTimestampRecordsForDateWithAdjacents(QDate date, QList<ExtTimestampRecord> records);
    QList<IntTimestampRecord> intTimestampRecordsForDateWithAdjacents(QDate date, QList<IntTimestampRecord> records);

private:
    DatabaseHandler *dbHandler = nullptr;
    QString _pathToDatabase = "";
    bool _indoor = true;
};

#endif // DAILYAVERAGECALCULATOR_H
