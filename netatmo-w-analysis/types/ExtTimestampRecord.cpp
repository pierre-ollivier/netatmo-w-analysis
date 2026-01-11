#include "ExtTimestampRecord.h"
#include "TimestampRecord.h"

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

QVariant ExtTimestampRecord::measurement(QString measurement) {
    return TimestampRecord::measurement(measurement);
}
