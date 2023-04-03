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


void tryDatabase2() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
    db.setDatabaseName("../netatmo-w-analysis/netatmo_analysis.db");
    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }
    qDebug() << db.tables();

    QSqlQuery query(db);
    query.prepare("INSERT INTO DailyRecords(Date,Tx,Tn) VALUES (?,?,?);");
    query.addBindValue(1678791600);
    query.addBindValue(13.8);
    query.addBindValue(5.1);
    query.exec();
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
//    tryDatabase2();
    MainWindow mainWin;
    mainWin.show();
    return app.exec();

}
