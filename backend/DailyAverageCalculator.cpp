#include "DailyAverageCalculator.h"
#include "DatabaseHandler.h"
#include "types/DailyRecord.h"

double getFirstTemperatureFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT temperature FROM ExtDailyRecords";
    query += "WHERE date = " + date.toString("dd/MM/yyyy");
    query += "ORDER BY timestamp";
    return dbHandler.getResultFromDatabase(query).toDouble();
}

double getLastTemperatureFromDate(QDate date) {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    QString query = "SELECT temperature FROM ExtDailyRecords";
    query += "WHERE date = " + date.toString("dd/MM/yyyy");
    query += "ORDER BY timestamp desc";
    return dbHandler.getResultFromDatabase(query).toDouble();
}

DailyAverageCalculator::DailyAverageCalculator(bool indoor)
{
    _indoor = indoor;
}
