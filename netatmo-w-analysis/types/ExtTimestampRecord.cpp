#include "ExtTimestampRecord.h"

ExtTimestampRecord::ExtTimestampRecord(
        long long timestamp,
        double temperature,
        int humidity
        ) : TimestampRecord(
                timestamp,
                temperature,
                humidity
                )
{

}
