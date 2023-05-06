#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QMenuBar>
#include "backend/NetatmoAPIHandler.h"
#include "backend/DatabaseHandler.h"
#include "backend/APIMonitor.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void buildWindow();
    void buildAPIHandler();
    void buildLabels();
    void buildButtons();
    void buildLayouts();
    void createMenus();
    void createActions();

public slots:
    void updateCurrentExtTemperature(double currentTemperature);
    void updateMinExtTemperature(double minTemperature);
    void updateMaxExtTemperature(double maxTemperature);
    void updateMinExtTemperatureTime(int timestamp);
    void updateMaxExtTemperatureTime(int timestamp);

    void updateCurrentIntTemperature(double currentTemperature);
    void updateMinIntTemperature(double minTemperature);
    void updateMaxIntTemperature(double maxTemperature);
    void updateMinIntTemperatureTime(int timestamp);
    void updateMaxIntTemperatureTime(int timestamp);

    void updateLastMeasurementDate(int timestamp);
    void updateActualisationDate(QDateTime timestamp);
    void updateRequestCounts();

    void addMonthData();
    void addMultipleMonthsData();

    void updateDailyIndoorDatabase();
    void updateDailyOutdoorDatabase();

private:
    // labels
    QLabel *currentExtTempLabel, *currentIntTempLabel;
    QLabel *statusLabel;
    QLabel *currentMinExtTempLabel, *currentMaxExtTempLabel;
    QLabel *currentMinIntTempLabel, *currentMaxIntTempLabel;

    QWidget *mainWidget;
    QMenuBar *menuBar;

    // buttons
    QPushButton *actualisationButton;

    //layouts
    QGridLayout *mainLayout;

    //other (provisional)
    NetatmoAPIHandler *apiHandler;

    //database handler
    DatabaseHandler *dbHandler;

    //other (provisional)
    QString accessToken = "";
    QLocale *deviceLocale;
    APIMonitor *apiMonitor;

    //actions
    QAction *requestCountsAction;
    QAction *addMonthDataAction;
    QAction *addMultipleMonthsDataAction;
    QAction *updateDailyIndoorDatabaseAction;
    QAction *updateDailyOutdoorDatabaseAction;
};

#endif // MAINWINDOW_H
