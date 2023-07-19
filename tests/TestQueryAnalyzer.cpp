#include "TestQueryAnalyzer.h"
#include <QTest>

TestQueryAnalyzer::TestQueryAnalyzer()
{
    analyzer = new QueryAnalyzer();
}

void TestQueryAnalyzer::testDateQueryFromMeasurementQuery() {
    QCOMPARE(analyzer->dateQueryFromMeasurementQuery(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2)"),
             "SELECT date FROM IndoorDailyRecords ORDER BY ((maxTemperature - minTemperature)/2)");

    QCOMPARE(analyzer->dateQueryFromMeasurementQuery(
                 "Select ((maxTemperature - minTemperature)/2) from IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2)"),
             "SELECT date FROM IndoorDailyRecords ORDER BY ((maxTemperature - minTemperature)/2)");
}

void TestQueryAnalyzer::testToASC() {
    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2)"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) ASC");

    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) ASC"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) ASC");

    QCOMPARE(analyzer->toASC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) DESC"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) ASC");
}

void TestQueryAnalyzer::testToDESC() {
    QCOMPARE(analyzer->toDESC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2)"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) DESC");

    QCOMPARE(analyzer->toDESC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) ASC"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) DESC");

    QCOMPARE(analyzer->toDESC(
                 "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
                 "ORDER BY ((maxTemperature - minTemperature)/2) DESC"),
             "SELECT ((maxTemperature - minTemperature)/2) FROM IndoorDailyRecords "
             "ORDER BY ((maxTemperature - minTemperature)/2) DESC");
}

void TestQueryAnalyzer::testMeasurementTypeFromQuery() {
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT maxTemperature FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "temperature");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT avgTemperature FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "temperature");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT (maxTemperature - minTemperature) FROM OutdoorDailyRecords"),
             "temperature");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT max(maxTemperature - minTemperature) FROM OutdoorDailyRecords WHERE month = 7"),
             "temperature");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT avgHumidity FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "humidity");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT maxDewPoint FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "dewpoint");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT avgHumidex FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "humidex");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT minPressure FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "pressure");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT avgCO2 FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "co2");
    QCOMPARE(analyzer->measurementTypeFromQuery(
                 "SELECT maxNoise FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "noise");
}

void TestQueryAnalyzer::testOperationFromQuery() {
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT maxTemperature FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "maximum");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT avgTemperature FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "average");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT (maxTemperature - minTemperature) FROM OutdoorDailyRecords"),
             "variation");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT max(maxTemperature - minTemperature) FROM OutdoorDailyRecords WHERE month = 7"),
             "maximum");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT avgHumidity FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "average");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT maxDewPoint FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "maximum");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT avgHumidex FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "average");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT minPressure FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "minimum");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT avgCO2 FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "average");
    QCOMPARE(analyzer->operationFromQuery(
                 "SELECT maxNoise FROM IndoorDailyRecords WHERE minHumidity = 25"),
             "maximum");
}

void TestQueryAnalyzer::testHorizontalLabelFromQuery() {
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT maxTemperature FROM IndoorDailyRecords WHERE minHumidity = 25", true),
             "Max. de la température max.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT avgTemperature FROM IndoorDailyRecords WHERE minHumidity = 25", false),
             "Min. de la température moy.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT (maxTemperature - minTemperature) FROM OutdoorDailyRecords", true),
             "Var. max. de la température");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT max(maxTemperature - minTemperature) FROM OutdoorDailyRecords WHERE month = 7", false),
             "Min. de la température max.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT avgHumidity FROM IndoorDailyRecords WHERE minHumidity = 25", true),
             "Max. de l'humidité moy.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT maxDewPoint FROM IndoorDailyRecords WHERE minHumidity = 25", false),
             "Min. du point de rosée max.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT avgHumidex FROM IndoorDailyRecords WHERE minHumidity = 25", true),
             "Max. de l'humidex moy.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT minPressure FROM IndoorDailyRecords WHERE minHumidity = 25", false),
             "Min. de la pression min.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT avgCO2 FROM IndoorDailyRecords WHERE minHumidity = 25", true),
             "Max. du CO2 moy.");
    QCOMPARE(analyzer->horizontalLabelFromQuery(
                 "SELECT maxNoise FROM IndoorDailyRecords WHERE minHumidity = 25", false),
             "Min. du bruit max.");
}
