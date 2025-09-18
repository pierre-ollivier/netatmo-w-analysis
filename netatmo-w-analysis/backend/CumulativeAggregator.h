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
    QMap<QDate, int> countMeasurementsMeetingCriteria(
        QString measurementType,
        QString measurementOption,
        QDate beginDate,
        QDate endDate,
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
    QMap<QDate, double> countMeasurementsMeetingCriteriaAveraged(
        QString measurementType,
        QString measurementOption,
        int beginMonth,
        int beginDay,
        int endMonth,
        int endDay,
        std::function<bool(double)> criteria,
        bool indoor,
        bool excludeCurrentYear = false
    );
    QMap<QDate, double> aggregateMeasurements(
        QString measurementType,
        QString measurementOption,
        int year,
        bool indoor,
        std::function<double(std::vector<QVariant>)> aggregationFunction,
        bool recoverMissingConstantValues = false
    );
    QMap<QDate, double> aggregateMeasurementsAveraged(
        QMap<int, QMap<QDate, double>> valuesByYear,
        bool includeCurrentYear = true
    );

private:
    DatabaseHandler *dbHandler;

    QString measurementQuery(QString measurementType, QString measurementOption, int year, bool indoor = false);
    QString measurementQuery(QString measurementType, QString measurementOption, QDate beginDate, QDate endDate, bool indoor = false);
    QString measurementQuery(
        QString measurementType,
        QString measurementOption,
        int beginMonth,
        int beginDay,
        int endMonth,
        int endDay,
        bool indoor = false,
        bool excludeCurrentYear = false
    );
    QString measurementQuery(QString measurementType, QString measurementOption, bool indoor = false, bool excludeCurrentYear = false);

    QString dateQuery(int year, bool indoor = false);
    QString dateQuery(QDate beginDate, QDate endDate, bool indoor = false);
    QString dateQuery(
        int beginMonth,
        int beginDay,
        int endMonth,
        int endDay,
        bool indoor = false,
        bool excludeCurrentYear = false
    );
    QString dateQuery(bool indoor = false, bool excludeCurrentYear = false);
};

#endif // CUMULATIVEAGGREGATOR_H
