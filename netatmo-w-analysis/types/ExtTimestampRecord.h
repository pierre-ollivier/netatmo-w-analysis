#ifndef EXTTIMESTAMPRECORD_H
#define EXTTIMESTAMPRECORD_H

#include "TimestampRecord.h"

class ExtTimestampRecord : public TimestampRecord
{
public:
    ExtTimestampRecord(
            long long timestamp,
            double temperature,
            int humidity
            );
};

#endif // EXTTIMESTAMPRECORD_H
