#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>


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

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    tryDatabase();
    return 0;
    return app.exec();

}
