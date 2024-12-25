#ifndef CUMULATIVEAGGREGATOR_H
#define CUMULATIVEAGGREGATOR_H

#include <QObject>

#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class CumulativeAggregator : public QObject
{
    Q_OBJECT
public:
    CumulativeAggregator(QObject *parent);

    QMap<QDate, int> countMaxTemperaturesHigherOrEqualThanThreshold(int year, double threshold);
    QMap<QDate, int> countMinTemperaturesHigherOrEqualThanThreshold(int year, double threshold);
    QMap<QDate, int> countAvgTemperaturesHigherOrEqualThanThreshold(int year, double threshold);
    QMap<QDate, int> countMeasurementsHigherOrEqualThanThreshold(QString measurementType, QString measurementOption, int year, double threshold);

private:
    DatabaseHandler *dbHandler;

    QString measurementQuery(QString measurementType, QString measurementOption, int year, bool indoor = false);
    QString dateQuery(int year, bool indoor = false);
};

#endif // CUMULATIVEAGGREGATOR_H
