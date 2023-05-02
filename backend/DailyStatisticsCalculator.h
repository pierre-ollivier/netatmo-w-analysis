#ifndef DAILYSTATISTICSCALCULATOR_H
#define DAILYSTATISTICSCALCULATOR_H

#include <QDate>
#include "DatabaseHandler.h"
#include "DailyAverageCalculator.h"

class DailyStatisticsCalculator
{
public:
    DailyStatisticsCalculator();

    //temperature

    double getMaxTemperatureFromDate(QDate date, bool indoor = false);
    long long getMaxTemperatureTimestampFromDate(QDate date, double maxTemperature, bool indoor = false);
    long long getMaxTemperatureTimestampFromDate(QDate date, bool indoor = false);

    double getMinTemperatureFromDate(QDate date, bool indoor = false);
    long long getMinTemperatureTimestampFromDate(QDate date, double minTemperature, bool indoor = false);
    long long getMinTemperatureTimestampFromDate(QDate date, bool indoor = false);

    double getAvgTemperatureFromDate(QDate date, bool indoor = false);

    // humidity

    int getMaxHumidityFromDate(QDate date, bool indoor = false);
    long long getMaxHumidityTimestampFromDate(QDate date, int maxHumidity, bool indoor = false);
    long long getMaxHumidityTimestampFromDate(QDate date, bool indoor = false);

    int getMinHumidityFromDate(QDate date, bool indoor = false);
    long long getMinHumidityTimestampFromDate(QDate date, int minHumidity, bool indoor = false);
    long long getMinHumidityTimestampFromDate(QDate date, bool indoor = false);

    double getAvgHumidityFromDate(QDate date, bool indoor = false);

    // dew point

    double getMaxDewPointFromDate(QDate date, bool indoor = false);
    long long getMaxDewPointTimestampFromDate(QDate date, double maxDewPoint, bool indoor = false);
    long long getMaxDewPointTimestampFromDate(QDate date, bool indoor = false);

    double getMinDewPointFromDate(QDate date, bool indoor = false);
    long long getMinDewPointTimestampFromDate(QDate date, double minDewPoint, bool indoor = false);
    long long getMinDewPointTimestampFromDate(QDate date, bool indoor = false);

    double getAvgDewPointFromDate(QDate date, bool indoor = false);

    // humidex

    double getMaxHumidexFromDate(QDate date, bool indoor = false);
    long long getMaxHumidexTimestampFromDate(QDate date, double maxDewPoint, bool indoor = false);
    long long getMaxHumidexTimestampFromDate(QDate date, bool indoor = false);

    double getMinHumidexFromDate(QDate date, bool indoor = false);
    long long getMinHumidexTimestampFromDate(QDate date, double minDewPoint, bool indoor = false);
    long long getMinHumidexTimestampFromDate(QDate date, bool indoor = false);

    double getAvgHumidexFromDate(QDate date, bool indoor = false);

    // pressure

    double getMaxPressureFromDate(QDate date);
    long long getMaxPressureTimestampFromDate(QDate date, double maxPressure);
    long long getMaxPressureTimestampFromDate(QDate date);

    double getMinPressureFromDate(QDate date);
    long long getMinPressureTimestampFromDate(QDate date, double minPressure);
    long long getMinPressureTimestampFromDate(QDate date);

    double getAvgPressureFromDate(QDate date);

    // co2

    int getMaxCO2FromDate(QDate date);
    long long getMaxCO2TimestampFromDate(QDate date, int maxCO2);
    long long getMaxCO2TimestampFromDate(QDate date);

    int getMinCO2FromDate(QDate date);
    long long getMinCO2TimestampFromDate(QDate date, int minCO2);
    long long getMinCO2TimestampFromDate(QDate date);

    double getAvgCO2FromDate(QDate date);

    // noise

    int getMaxNoiseFromDate(QDate date);
    long long getMaxNoiseTimestampFromDate(QDate date, int maxNoise);
    long long getMaxNoiseTimestampFromDate(QDate date);

    int getMinNoiseFromDate(QDate date);
    long long getMinNoiseTimestampFromDate(QDate date, int minNoise);
    long long getMinNoiseTimestampFromDate(QDate date);

    double getAvgNoiseFromDate(QDate date);

    // others

    long long getFirstTimestampFromDate(QDate date);
    long long getFirstTimestampFromDateWithUTCOffset(QDate date, int offsetFromUTCInHours);

private:
    DatabaseHandler *dbHandler;
    DailyAverageCalculator *indoorAverageCalculator;
    DailyAverageCalculator *outdoorAverageCalculator;
};

#endif // DAILYSTATISTICSCALCULATOR_H
