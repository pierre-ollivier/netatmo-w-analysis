#ifndef CUMULATIVEAGGREGATOR_H
#define CUMULATIVEAGGREGATOR_H

#include <QObject>

#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class CumulativeAggregator : public QObject
{
    Q_OBJECT
public:
    CumulativeAggregator(QObject *parent);

    QMap<QDate, int> countMeasurementsMeetingCriteria(
        QString measurementType,
        QString measurementOption,
        int year,
        std::function<bool(double)> criteria,
        bool indoor
    );
    QMap<QDate, double> countMeasurementsMeetingCriteriaAveraged(
        QString measurementType,
        QString measurementOption,
        std::function<bool(double)> criteria,
        bool indoor,
        bool excludeCurrentYear = false
    );
    QMap<QDate, double> aggregateMeasurements(
        QString measurementType,
        QString measurementOption,
        int year,
        bool indoor,
        std::function<double(std::vector<QVariant>)> aggregationFunction
    );

private:
    DatabaseHandler *dbHandler;

    QString measurementQuery(QString measurementType, QString measurementOption, int year, bool indoor = false);
    QString measurementQuery(QString measurementType, QString measurementOption, bool indoor = false, bool excludeCurrentYear = false);

    QString dateQuery(int year, bool indoor = false);
    QString dateQuery(bool indoor = false, bool excludeCurrentYear = false);
};

#endif // CUMULATIVEAGGREGATOR_H
