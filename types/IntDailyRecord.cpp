#include "IntDailyRecord.h"
#include "DailyRecord.cpp"
#include <QDateTime>

IntDailyRecord::IntDailyRecord() : DailyRecord() // put here the transmitted parameters
{

}

double IntDailyRecord::maxPressure() {
    return _maxPressure;
}
double IntDailyRecord::minPressure() {
    return _minPressure;
}
double IntDailyRecord::avgPressure() {
    return _avgPressure;
}

int IntDailyRecord::maxPressureTimestamp() {
    return _maxPressureTimestamp;
}
int IntDailyRecord::minPressureTimestamp() {
    return _minPressureTimestamp;
}

QTime IntDailyRecord::maxPressureTime() {
    return timestampToTime(_maxPressureTimestamp);
}
QTime IntDailyRecord::minPressureTime() {
    return timestampToTime(_minPressureTimestamp);
}
