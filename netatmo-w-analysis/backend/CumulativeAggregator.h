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

private:
    DatabaseHandler *dbHandler;

    QString measurementQuery(QString measurementType, QString measurementOption, int year, bool indoor = false);
    QString dateQuery(int year, bool indoor = false);
};

#endif // CUMULATIVEAGGREGATOR_H
