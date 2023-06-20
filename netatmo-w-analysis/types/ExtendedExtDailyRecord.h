#ifndef EXTENDEDEXTDAILYRECORD_H
#define EXTENDEDEXTDAILYRECORD_H

#include "../netatmo-w-analysis/types/ExtDailyRecord.h"

class ExtendedExtDailyRecord
{
public:
    ExtendedExtDailyRecord();

    ExtDailyRecord extDailyRecord();
    double minTemperature();
    double maxTemperature();
    bool extDailyRecordDefined();
    bool minTemperatureDefined();
    bool maxTemperatureDefined();
    bool allDefined();

    void setExtDailyRecord(ExtDailyRecord *record);
    void setMinTemperature(double minTemperature);
    void setMaxTemperature(double maxTemperature);

    ExtDailyRecord wrap();

private:
    ExtDailyRecord *_extDailyRecord = new ExtDailyRecord(QDate(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    double _minTemperature = 0;
    double _maxTemperature = 0;
    bool _extDailyRecordDefined = false;
    bool _maxTemperatureDefined = false;
    bool _minTemperatureDefined = false;
};

#endif // EXTENDEDEXTDAILYRECORD_H
