#ifndef DAILYSTATISTICSCALCULATOR_H
#define DAILYSTATISTICSCALCULATOR_H

#include <QDate>
#include <QObject>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"
#include "../netatmo-w-analysis/backend/DailyAverageCalculator.h"

class DailyStatisticsCalculator : public QObject
{
public:
    DailyStatisticsCalculator(QString pathToDatabase, DatabaseHandler *dbHandler);
    DailyStatisticsCalculator(QObject *parent, QString pathToDatabase, DatabaseHandler *dbHandler);

    //temperature

    double getMaxTemperatureFromDate(QDate date, bool indoor = false);
    long long getMaxTemperatureTimestampFromDate(QDate date, double maxTemperature, bool indoor = false);
    long long getMaxTemperatureTimestampFromDate(QDate date, bool indoor = false);
    QPair<double, long long> getMaxTemperatureInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getMinTemperatureFromDate(QDate date, bool indoor = false);
    long long getMinTemperatureTimestampFromDate(QDate date, double minTemperature, bool indoor = false);
    long long getMinTemperatureTimestampFromDate(QDate date, bool indoor = false);
    QPair<double, long long> getMinTemperatureInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getAvgTemperatureFromDate(QDate date, bool indoor = false);

    // humidity

    int getMaxHumidityFromDate(QDate date, bool indoor = false);
    long long getMaxHumidityTimestampFromDate(QDate date, int maxHumidity, bool indoor = false);
    long long getMaxHumidityTimestampFromDate(QDate date, bool indoor = false);
    QPair<int, long long> getMaxHumidityInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    int getMinHumidityFromDate(QDate date, bool indoor = false);
    long long getMinHumidityTimestampFromDate(QDate date, int minHumidity, bool indoor = false);
    long long getMinHumidityTimestampFromDate(QDate date, bool indoor = false);
    QPair<int, long long> getMinHumidityInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getAvgHumidityFromDate(QDate date, bool indoor = false);

    // dew point

    double getMaxDewPointFromDate(QDate date, bool indoor = false);
    long long getMaxDewPointTimestampFromDate(QDate date, double maxDewPoint, bool indoor = false);
    long long getMaxDewPointTimestampFromDate(QDate date, bool indoor = false);
    QPair<double, long long> getMaxDewPointInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getMinDewPointFromDate(QDate date, bool indoor = false);
    long long getMinDewPointTimestampFromDate(QDate date, double minDewPoint, bool indoor = false);
    long long getMinDewPointTimestampFromDate(QDate date, bool indoor = false);
    QPair<double, long long> getMinDewPointInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getAvgDewPointFromDate(QDate date, bool indoor = false);

    // humidex

    double getMaxHumidexFromDate(QDate date, bool indoor = false);
    long long getMaxHumidexTimestampFromDate(QDate date, double maxDewPoint, bool indoor = false);
    long long getMaxHumidexTimestampFromDate(QDate date, bool indoor = false);
    QPair<double, long long> getMaxHumidexInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getMinHumidexFromDate(QDate date, bool indoor = false);
    long long getMinHumidexTimestampFromDate(QDate date, double minDewPoint, bool indoor = false);
    long long getMinHumidexTimestampFromDate(QDate date, bool indoor = false);
    QPair<double, long long> getMinHumidexInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getAvgHumidexFromDate(QDate date, bool indoor = false);

    // pressure

    double getMaxPressureFromDate(QDate date);
    long long getMaxPressureTimestampFromDate(QDate date, double maxPressure);
    long long getMaxPressureTimestampFromDate(QDate date);
    QPair<double, long long> getMaxPressureInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getMinPressureFromDate(QDate date);
    long long getMinPressureTimestampFromDate(QDate date, double minPressure);
    long long getMinPressureTimestampFromDate(QDate date);
    QPair<double, long long> getMinPressureInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getAvgPressureFromDate(QDate date);

    // co2

    int getMaxCO2FromDate(QDate date);
    long long getMaxCO2TimestampFromDate(QDate date, int maxCO2);
    long long getMaxCO2TimestampFromDate(QDate date);
    QPair<int, long long> getMaxCO2InfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    int getMinCO2FromDate(QDate date);
    long long getMinCO2TimestampFromDate(QDate date, int minCO2);
    long long getMinCO2TimestampFromDate(QDate date);
    QPair<int, long long> getMinCO2InfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getAvgCO2FromDate(QDate date);

    // noise

    int getMaxNoiseFromDate(QDate date);
    long long getMaxNoiseTimestampFromDate(QDate date, int maxNoise);
    long long getMaxNoiseTimestampFromDate(QDate date);
    QPair<int, long long> getMaxNoiseInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    int getMinNoiseFromDate(QDate date);
    long long getMinNoiseTimestampFromDate(QDate date, int minNoise);
    long long getMinNoiseTimestampFromDate(QDate date);
    QPair<int, long long> getMinNoiseInfoFromDate(QDate date, QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records);

    double getAvgNoiseFromDate(QDate date);

    // measurement

    QPair<QVariant, long long> getMaxMeasurementInfoFromDate(
        QDate date,
        QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records,
        QPair<std::function<QVariant(ExtTimestampRecord)>, std::function<QVariant(IntTimestampRecord)>> measurementGetter,
        bool localTime = true,
        int utcOffsetH = 0
        );

    QPair<QVariant, long long> getMinMeasurementInfoFromDate(
        QDate date,
        QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> records,
        QPair<std::function<QVariant(ExtTimestampRecord)>, std::function<QVariant(IntTimestampRecord)>> measurementGetter,
        bool localTime = true,
        int utcOffsetH = 0
        );

    // others

    long long getFirstTimestampFromDate(QDate date);
    long long getFirstTimestampFromDateWithUTCOffset(QDate date, int offsetFromUTCInHours);

private:
    QString _pathToDatabase;
    DatabaseHandler *_dbHandler;
    DailyAverageCalculator *indoorAverageCalculator;
    DailyAverageCalculator *outdoorAverageCalculator;
};

#endif // DAILYSTATISTICSCALCULATOR_H
