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
    QVariant measurement(QString measurement);
};

#endif // EXTTIMESTAMPRECORD_H
