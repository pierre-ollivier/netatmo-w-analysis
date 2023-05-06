#include <QApplication>
#include "frontend/MainWindow.h"
#include "playground.cpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    mainWin.setWindowTitle("netatmo-w-analysis v1.0.0");
    executeAllPlaygroundFunctions();
    return app.exec();
}
