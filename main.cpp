#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    qDebug() << "Hello Git from Qt Creator!";
    return app.exec();

}
