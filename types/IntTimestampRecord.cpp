#include "IntTimestampRecord.h"

IntTimestampRecord::IntTimestampRecord(
        long long timestamp,
        double temperature,
        int humidity,
        double pressure,
        int co2,
        int noise
        ) : TimestampRecord(
                timestamp,
                temperature,
                humidity
                )
{
    _pressure = pressure;
    _co2 = co2;
    _noise = noise;
}

double IntTimestampRecord::pressure() {
    return _pressure;
}
int IntTimestampRecord::co2() {
    return _co2;
}
int IntTimestampRecord::noise() {
    return _noise;
}
