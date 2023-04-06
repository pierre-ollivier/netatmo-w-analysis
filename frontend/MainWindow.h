#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "backend/NetatmoAPIHandler.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    void buildWindow();
    void buildLabels();
    void buildLayouts();

public slots:
    void logToken(QString token); // provisional of course
    void updateCurrentExtTemperature(double currentTemperature);

private:
    // labels
    QLabel *labelCurrentTemp, *labelCurrentTempInt;
    QLabel *labelStatus;
    QLabel *labelDailyTmin, *labelDailyTmax;
    QLabel *labelDailyTminInt, *labelDailyTmaxInt;

    //layouts
    QGridLayout *mainLayout;

    //other (provisional)
    NetatmoAPIHandler *apiHandler;

    //other (provisional)
    QString accessToken = "";
};

#endif // MAINWINDOW_H
