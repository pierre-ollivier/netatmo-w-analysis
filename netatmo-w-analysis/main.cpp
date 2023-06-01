#include <QApplication>
#include <QFile>
#include "frontend/MainWindow.h"
#include "playground.cpp"

extern const QString VERSION;
extern const QString PATH_TO_PROD_DATABASE;
extern const QString PATH_TO_COPY_DATABASE;
extern const QString APP_PATH;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    mainWin.setWindowTitle("netatmo-w-analysis v" + VERSION);

    executeAllPlaygroundFunctions();

    bool valid = QFile::copy(APP_PATH + "/" + PATH_TO_PROD_DATABASE,
                             APP_PATH + "/" + PATH_TO_COPY_DATABASE);

    if (valid) {
        int result = app.exec();
        QFile copyDatabase(APP_PATH + "/" + PATH_TO_COPY_DATABASE);
        bool deleteValid = copyDatabase.remove();
        if (deleteValid) return result;
        else {
            qDebug() << "Warning: the copied database couldn't be deleted.";
            qDebug() << copyDatabase.errorString();
            return 2;
        }
    }

    else {
        return 1;
    }
}
