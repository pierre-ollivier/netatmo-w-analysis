#ifndef TESTDAILYSTATISTICSCALCULATOR_H
#define TESTDAILYSTATISTICSCALCULATOR_H

#include "../netatmo-w-analysis/backend/DailyStatisticsCalculator.h"

#include <QObject>

class TestDailyStatisticsCalculator : public QObject
{
    Q_OBJECT

public:
    TestDailyStatisticsCalculator();

private slots:

    //temperature

    void testGetMaxTemperatureFromDate();
    void testGetMaxTemperatureTimestampFromDate();

    void testGetMinTemperatureFromDate();
    void testGetMinTemperatureTimestampFromDate();

    void testGetAvgTemperatureFromDate();

    // humidity

    void testGetMaxHumidityFromDate();
    void testGetMaxHumidityTimestampFromDate();

    void testGetMinHumidityFromDate();
    void testGetMinHumidityTimestampFromDate();

    void testGetAvgHumidityFromDate();

    // dew point

    void testGetMaxDewPointFromDate();
    void testGetMaxDewPointTimestampFromDate();

    void testGetMinDewPointFromDate();
    void testGetMinDewPointTimestampFromDate();

    void testGetAvgDewPointFromDate();

    // humidex

    void testGetMaxHumidexFromDate();
    void testGetMaxHumidexTimestampFromDate();

    void testGetMinHumidexFromDate();
    void testGetMinHumidexTimestampFromDate();

    void testGetAvgHumidexFromDate();

    // pressure

    void testGetMaxPressureFromDate();
    void testGetMaxPressureTimestampFromDate();

    void testGetMinPressureFromDate();
    void testGetMinPressureTimestampFromDate();

    void testGetAvgPressureFromDate();

    // co2

    void testGetMaxCO2FromDate();

    void testGetMinCO2FromDate();

    void testGetAvgCO2FromDate();

    // noise

    void testGetMaxNoiseFromDate();

    void testGetMinNoiseFromDate();

    void testGetAvgNoiseFromDate();

    // others

    void testGetFirstTimestampFromDate();
    void testGetFirstTimestampFromDateWithUTCOffset();

private:
    DailyStatisticsCalculator *calculator;
};

#endif // TESTDAILYSTATISTICSCALCULATOR_H
