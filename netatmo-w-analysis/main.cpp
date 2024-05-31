#include <QApplication>
#include <QFile>
#include "frontend/MainWindow.h"
#include "backend/DatabaseHandler.h"
#include "backend/NetatmoAPIHandler.h"
#include "playground.cpp"

extern const QString VERSION;
extern const QString PATH_TO_PROD_DATABASE;
extern const QString PATH_TO_COPY_DATABASE;
extern const QString APP_PATH;

int main(int argc, char *argv[]) {

    bool valid = QFile::copy(PATH_TO_PROD_DATABASE,
                             PATH_TO_COPY_DATABASE);

    if (valid) {
        QApplication app(argc, argv);
        MainWindow mainWin;
        mainWin.show();
        mainWin.setWindowTitle("netatmo-w-analysis v" + VERSION);

        executeAllPlaygroundFunctions();
        int result = 3;

        try {
            result = app.exec();
        }

        catch (const std::exception& ex) {
            qDebug() << ex.what();
        }

        QFile copyDatabase(PATH_TO_COPY_DATABASE);
        copyDatabase.close();
        bool deleteValid = copyDatabase.remove();
        if (deleteValid) return result;
        else {
            qDebug() << "Warning: the copied database couldn't be deleted.";
            qDebug() << copyDatabase.errorString();
            return 2;
        }

    }

    else {
        qDebug() << "Error: impossible to copy the database from" << PATH_TO_PROD_DATABASE << "to" << PATH_TO_COPY_DATABASE;
        return 1;
    }
}
