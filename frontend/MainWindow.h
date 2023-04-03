#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "backend/NetatmoAPIHandler.h"


class MainWindow : public QWidget
{
public:
    MainWindow();
    void buildWindow();
    void buildLabels();
    void buildLayouts();

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
};

#endif // MAINWINDOW_H
