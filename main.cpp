#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QWidget>
#include <QtWidgets>
#include <QNetworkRequest>
#include <QNetworkAccessManager>


void tryDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection_name");
    db.setDatabaseName("../netatmo-w-analysis/test_sqlite.db");
    if (!db.open()) {
        qDebug() << "Database open error";
    }
    if (!db.isOpen() ) {
        qDebug() << "Database is not open";
    }
    qDebug() << db.tables();
    QSqlQuery query(db);
    query.exec("SELECT first_name, last_name FROM actor");
    qDebug() << query.next();

    while (query.next()) {
        qDebug() << query.value(1).toString();
    }

    qDebug() << "End";
}

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
    //query.exec("INSERT INTO DailyRecords VALUES (null,null,null,null);");
    query.prepare("INSERT INTO DailyRecords(Date,Tx,Tn) VALUES (?,?,?);");
    //query.addBindValue(14);
    query.addBindValue(1678791600);
    query.addBindValue(13.8);
    query.addBindValue(5.1);
    query.exec();
}

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    tryDatabase2();
    return 0;
    return app.exec();

}


