#ifndef INTDAILYRECORD_H
#define INTDAILYRECORD_H

#include "DailyRecord.h"

class IntDailyRecord : public DailyRecord
{
public:
    IntDailyRecord();

    double maxPressure();
    double minPressure();
    double avgPressure();

    int maxPressureTimestamp();
    int minPressureTimestamp();

    QTime maxPressureTime();
    QTime minPressureTime();

private:
    double _maxPressure;
    double _minPressure;
    double _avgPressure;

    int _maxPressureTimestamp;
    int _minPressureTimestamp;
};

#endif // INTDAILYRECORD_H
