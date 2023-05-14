#include <QApplication>
#include "frontend/MainWindow.h"
#include "playground.cpp"

int main(int argc, char *argv[]) {
    extern const QString VERSION;
    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    mainWin.setWindowTitle("netatmo-w-analysis v" + VERSION);
    executeAllPlaygroundFunctions();
    return app.exec();
}
