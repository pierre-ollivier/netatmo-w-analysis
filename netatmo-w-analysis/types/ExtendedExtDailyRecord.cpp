#include "ExtendedExtDailyRecord.h"
#include <QDebug>
ExtendedExtDailyRecord::ExtendedExtDailyRecord()
{

}

ExtDailyRecord ExtendedExtDailyRecord::extDailyRecord() {
    return *_extDailyRecord;
}
double ExtendedExtDailyRecord::minTemperature() {
    return _minTemperature;
}
double ExtendedExtDailyRecord::maxTemperature() {
    return _maxTemperature;
}
bool ExtendedExtDailyRecord::extDailyRecordDefined() {
    return _extDailyRecordDefined;
}
bool ExtendedExtDailyRecord::minTemperatureDefined() {
    return _minTemperatureDefined;
}
bool ExtendedExtDailyRecord::maxTemperatureDefined() {
    return _maxTemperatureDefined;
}
bool ExtendedExtDailyRecord::allDefined() {
    return extDailyRecordDefined() && maxTemperatureDefined() && minTemperatureDefined();
}

void ExtendedExtDailyRecord::setExtDailyRecord(ExtDailyRecord *record) {
    _extDailyRecord = record;
    _extDailyRecordDefined = true;
}
void ExtendedExtDailyRecord::setMinTemperature(double minTemperature) {
    _minTemperature = minTemperature;
    _minTemperatureDefined = true;
}
void ExtendedExtDailyRecord::setMaxTemperature(double maxTemperature) {
    _maxTemperature = maxTemperature;
    _maxTemperatureDefined = true;
}

ExtDailyRecord ExtendedExtDailyRecord::wrap() {
    ExtDailyRecord *result = new ExtDailyRecord(*_extDailyRecord);
    result->setMaxTemperature(_maxTemperature);
    result->setMinTemperature(_minTemperature);
    return *result;
}
