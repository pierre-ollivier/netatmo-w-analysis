#include "IntDailyRecord.h"
#include <QDateTime>

QTime timestampToTime(int timestampInms) {
    QDateTime dt;
    dt.setMSecsSinceEpoch(timestampInms);
    return dt.time();
}

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
