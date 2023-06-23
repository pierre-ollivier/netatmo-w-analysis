#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QMenuBar>
#include <QtCharts>
#include "../netatmo-w-analysis/backend/NetatmoAPIHandler.h"
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"
#include "../netatmo-w-analysis/backend/APIMonitor.h"
#include "../netatmo-w-analysis/backend/OldDataUploader.h"
#include "../netatmo-w-analysis/frontend/HomePageChart.h"
#include "../netatmo-w-analysis/frontend/EphemerisPanel.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void buildWindow();
    void buildAPIHandler();
    void buildLabels();
    void buildButtons();
    void buildCharts();
    void buildEphemerisPanel();
    void buildLayouts();
    void createMenus();
    void createActions();

public slots:
    void setAccessToken(QString);

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

    void updateIndoorChart(QString measurementType = "", int durationInHours = 0);
    void updateOutdoorChart(QString measurementType = "", int durationInHours = 0);

    void addMonthData();
    void addMultipleMonthsData();

    void updateDailyIndoorDatabase();
    void updateDailyOutdoorDatabase();

    void displayMonthlyReport();
    void displayYearlyReport();

    void addDataFromCurrentMonths();
    void changeChartsOptions();

    void showNormals();

signals:


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

    //database handlers
    DatabaseHandler *dbHandlerProd;
    DatabaseHandler *dbHandlerCopy;

    //other (provisional)
    QString accessToken = "";
    QLocale *deviceLocale;
    APIMonitor *apiMonitor;
    HomePageChart *indoorChart;
    HomePageChart *outdoorChart;

    //data uploader
    OldDataUploader *oldDataUploader;

    //actions
    QAction *requestCountsAction;
    QAction *addMonthDataAction;
    QAction *addMultipleMonthsDataAction;
    QAction *updateDailyIndoorDatabaseAction;
    QAction *updateDailyOutdoorDatabaseAction;
    QAction *displayMonthlyReportAction;
    QAction *displayYearlyReportAction;
    QAction *normalsAction;

    //chart options
    QRadioButton *h4Option;
    QRadioButton *h24Option;
    QRadioButton *h192Option;
    QHBoxLayout *chartsDurationOptionsLayout;
    QGroupBox *chartsDurationOptionsGroupBox;

    QRadioButton *temperatureOption;
    QRadioButton *humidityOption;
    QRadioButton *dewPointOption;
    QRadioButton *humidexOption;
    QHBoxLayout *chartsMeasurementOptionsLayout;
    QGroupBox *chartsMeasurementOptionsGroupBox;

    // right panel with ephemeris
    EphemerisPanel *ephemerisPanel;

    QString _measurementType = "temperature";
    int _durationInHours = 4;


};

#endif // MAINWINDOW_H
