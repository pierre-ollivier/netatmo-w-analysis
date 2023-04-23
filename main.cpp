#include <QApplication>
#include "frontend/MainWindow.h"
#include "playground.cpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    executeAllPlaygroundFunctions();
    return app.exec();

}
