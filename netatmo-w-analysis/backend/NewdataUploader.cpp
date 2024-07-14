#include "NewDataUploader.h"
#include <QProgressDialog>

#include "../netatmo-w-analysis/types/ExtDailyRecord.h"
#include "../netatmo-w-analysis/types/IntDailyRecord.h"

NewDataUploader::NewDataUploader(DatabaseHandler *dbHandler, DailyStatisticsCalculator *dailyCalculator) {
    _dbHandler = dbHandler;
    _dailyCalculator = dailyCalculator;
}

NewDataUploader::NewDataUploader(QObject *parent, DatabaseHandler *dbHandler, DailyStatisticsCalculator *dailyCalculator) : QObject(parent) {
    _dbHandler = dbHandler;
    _dailyCalculator = dailyCalculator;
}

void NewDataUploader::uploadOutdoorDailyRecords(QDate beginDate, QDate endDate, bool verbose) {
    QProgressDialog progress("Ajout des nouvelles données...", "Annuler", beginDate.toJulianDay(), endDate.toJulianDay());
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(beginDate.toJulianDay());
    if (!verbose) progress.reset();
    for (QDate date = beginDate; date <= endDate; date = date.addDays(1)) {
        ExtDailyRecord record(
            date,
            _dailyCalculator->getMaxTemperatureFromDate(date),
            _dailyCalculator->getMinTemperatureFromDate(date),
            _dailyCalculator->getAvgTemperatureFromDate(date),
            _dailyCalculator->getMaxHumidityFromDate(date),
            _dailyCalculator->getMinHumidityFromDate(date),
            _dailyCalculator->getAvgHumidityFromDate(date),
            _dailyCalculator->getMaxDewPointFromDate(date),
            _dailyCalculator->getMinDewPointFromDate(date),
            _dailyCalculator->getAvgDewPointFromDate(date),
            _dailyCalculator->getMaxHumidexFromDate(date),
            _dailyCalculator->getMinHumidexFromDate(date),
            _dailyCalculator->getAvgHumidexFromDate(date),
            _dailyCalculator->getMaxTemperatureTimestampFromDate(date),
            _dailyCalculator->getMinTemperatureTimestampFromDate(date),
            _dailyCalculator->getMaxHumidityTimestampFromDate(date),
            _dailyCalculator->getMinHumidityTimestampFromDate(date),
            _dailyCalculator->getMaxDewPointTimestampFromDate(date),
            _dailyCalculator->getMinDewPointTimestampFromDate(date),
            _dailyCalculator->getMaxHumidexTimestampFromDate(date),
            _dailyCalculator->getMinHumidexTimestampFromDate(date)
            );
        _dbHandler->postOutdoorDailyRecord(record, "OutdoorDailyRecords");
        if (verbose) progress.setValue(date.toJulianDay());
    }
}

void NewDataUploader::uploadIndoorDailyRecords(QDate beginDate, QDate endDate, bool verbose) {
    QProgressDialog progress("Ajout des nouvelles données...", "Annuler", beginDate.toJulianDay(), endDate.toJulianDay());
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(beginDate.toJulianDay());
    if (!verbose) progress.reset();
    for (QDate date = beginDate; date <= endDate; date = date.addDays(1)) {
        IntDailyRecord record(
            date,
            _dailyCalculator->getMaxTemperatureFromDate(date, true),
            _dailyCalculator->getMinTemperatureFromDate(date, true),
            _dailyCalculator->getAvgTemperatureFromDate(date, true),
            _dailyCalculator->getMaxHumidityFromDate(date, true),
            _dailyCalculator->getMinHumidityFromDate(date, true),
            _dailyCalculator->getAvgHumidityFromDate(date, true),
            _dailyCalculator->getMaxDewPointFromDate(date, true),
            _dailyCalculator->getMinDewPointFromDate(date, true),
            _dailyCalculator->getAvgDewPointFromDate(date, true),
            _dailyCalculator->getMaxHumidexFromDate(date, true),
            _dailyCalculator->getMinHumidexFromDate(date, true),
            _dailyCalculator->getAvgHumidexFromDate(date, true),
            _dailyCalculator->getMaxPressureFromDate(date),
            _dailyCalculator->getMinPressureFromDate(date),
            _dailyCalculator->getAvgPressureFromDate(date),
            _dailyCalculator->getMaxCO2FromDate(date),
            _dailyCalculator->getMinCO2FromDate(date),
            _dailyCalculator->getAvgCO2FromDate(date),
            _dailyCalculator->getMaxNoiseFromDate(date),
            _dailyCalculator->getMinNoiseFromDate(date),
            _dailyCalculator->getAvgNoiseFromDate(date),
            _dailyCalculator->getMaxTemperatureTimestampFromDate(date, true),
            _dailyCalculator->getMinTemperatureTimestampFromDate(date, true),
            _dailyCalculator->getMaxHumidityTimestampFromDate(date, true),
            _dailyCalculator->getMinHumidityTimestampFromDate(date, true),
            _dailyCalculator->getMaxDewPointTimestampFromDate(date, true),
            _dailyCalculator->getMinDewPointTimestampFromDate(date, true),
            _dailyCalculator->getMaxHumidexTimestampFromDate(date, true),
            _dailyCalculator->getMinHumidexTimestampFromDate(date, true),
            _dailyCalculator->getMaxPressureTimestampFromDate(date),
            _dailyCalculator->getMinPressureTimestampFromDate(date)
            );
        _dbHandler->postIndoorDailyRecord(record, "IndoorDailyRecords");
        if (verbose) progress.setValue(date.toJulianDay());
    }
}
