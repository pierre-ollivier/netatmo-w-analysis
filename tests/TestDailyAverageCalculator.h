#ifndef TESTDAILYAVERAGECALCULATOR_H
#define TESTDAILYAVERAGECALCULATOR_H

#include <QObject>
#include "../netatmo-w-analysis/backend/DailyAverageCalculator.h"

class TestDailyAverageCalculator : public QObject
{
    Q_OBJECT

public:
    TestDailyAverageCalculator();

private slots:
    void testGetFirstMeasurementFromDate();
    void testGetLastMeasurementFromDate();
    void testGetFirstTimestampFromDate();
    void testGetLastTimestampFromDate();

private:
    DailyAverageCalculator *indoorCalculator;
    DailyAverageCalculator *outdoorCalculator;
};

#endif // TESTDAILYAVERAGECALCULATOR_H
