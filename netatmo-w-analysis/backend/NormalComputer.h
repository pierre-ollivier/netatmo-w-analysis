#ifndef NORMALCOMPUTER_H
#define NORMALCOMPUTER_H

#include <QDate>
#include <QObject>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class NormalComputer : public QObject
{
public:
    NormalComputer(DatabaseHandler *dbHandler);
    NormalComputer(QObject *parent, DatabaseHandler *dbHandler);
    int minYear(QString tableName);
    double normalMeasurementByMovingAverage(
            QString tableName,
            QDate date,
            QString measurement,
            int daysCount);

    double stdevMeasurementByMovingAverage(
            QString tableName,
            QDate date,
            QString measurement,
            int daysCount);

    double stdevMeasurementByMovingAverage(
            QString tableName,
            QDate date,
            QString measurement,
            double average,
            int daysCount);

    QList<double> createAveragesList(QString tableName,
                                     QString measurement,
                                     int daysCount);

    QList<double> createStandardDeviationList(QString tableName,
                                              QString measurement,
                                              int daysCount);

    QList<double> createStandardDeviationList(QString tableName,
                                              QString measurement,
                                              QList<double> averages,
                                              int daysCount);

    QList<double> createValuesFromCurrentYear(QString tableName,
                                              QString measurement);

    QList<double> createValuesFromGivenYear(int year,
                                            QString tableName,
                                            QString measurement);

private:
    DatabaseHandler *_dbHandler;
};

#endif // NORMALCOMPUTER_H
