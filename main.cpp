#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QWidget>
#include <QtWidgets>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "frontend/MainWindow.h"
#include "backend/DatabaseHandler.h"
#include "types/DailyRecord.h"


void tryDatabase2() {
    DatabaseHandler dbHandler("netatmo_analysis.db");
    dbHandler.postDailyRecord(
                DailyRecord(
                    QDate(2023, 4, 14),
                    9.7,
                    6.3,
                    8.3,
                    93,
                    68,
                    81.2,
                    1681471380000,
                    1681450020000,
                    1681487760000,
                    1681423440000
                    ),
                "OutdoorDailyRecords");

}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    tryDatabase2();
    MainWindow mainWin;
    mainWin.show();
    return app.exec();

}
