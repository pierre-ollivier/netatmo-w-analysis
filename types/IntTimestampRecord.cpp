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

double IntTimestampRecord::getPressure() {
    return _pressure;
}
int IntTimestampRecord::getCO2() {
    return _co2;
}
int IntTimestampRecord::getNoise() {
    return _noise;
}
