#ifndef NEWDATAUPLOADER_H
#define NEWDATAUPLOADER_H

#include <QDate>
#include <QObject>

#include "../netatmo-w-analysis/backend/DatabaseHandler.h"
#include "../netatmo-w-analysis/backend/DailyStatisticsCalculator.h"

class NewDataUploader : public QObject
{
public:
    NewDataUploader(DatabaseHandler *dbHandler, DailyStatisticsCalculator *dailyCalculator);
    NewDataUploader(QObject *parent, DatabaseHandler *dbHandler, DailyStatisticsCalculator *dailyCalculator);

    void uploadOutdoorDailyRecords(QDate beginDate, QDate endDate, bool verbose = true);
    void uploadIndoorDailyRecords(QDate beginDate, QDate endDate, bool verbose = true);

private:
    DatabaseHandler *_dbHandler;
    DailyStatisticsCalculator *_dailyCalculator;
};

#endif // NEWDATAUPLOADER_H
