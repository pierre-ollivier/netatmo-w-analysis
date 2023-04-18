#include <QApplication>
#include "frontend/MainWindow.h"
#include "backend/DatabaseHandler.h"
#include "types/ExtDailyRecord.h"

void tryPostFromCsv() {
    // This function works and takes around 10 seconds to run
#include "backend/DatabaseHandler.h"
    DatabaseHandler dbHandler("netatmo_analysis.db");
    dbHandler.postFromOutdoorCsv("2023-02.csv", "OutdoorTimestampRecords");

}

void tryDatabase2() {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    dbHandler.postOutdoorDailyRecord(
                ExtDailyRecord(
                    QDate(2023, 4, 14),
                    9.7,
                    6.3,
                    8.3,
                    93,
                    68,
                    81.2,
                    1681471380,
                    1681450020,
                    1681487760,
                    1681423440
                    ),
                "OutdoorDailyRecords");

}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
//    tryPostFromCsv();
    tryDatabase2();
    MainWindow mainWin;
    mainWin.show();
    return app.exec();

}
