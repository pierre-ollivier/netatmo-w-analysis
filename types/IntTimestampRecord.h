#ifndef INTTIMESTAMPRECORD_H
#define INTTIMESTAMPRECORD_H

#include "TimestampRecord.h"


class IntTimestampRecord : public TimestampRecord
{
public:
public:
    IntTimestampRecord(
            long long timestamp,
            double temperature,
            int humidity,
            double pressure,
            int co2,
            int noise
            );

    double getPressure();
    int getCO2();
    int getNoise();

private:
    double _pressure;
    int _co2;
    int _noise;
};

#endif // INTTIMESTAMPRECORD_H
