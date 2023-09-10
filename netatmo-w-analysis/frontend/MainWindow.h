#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QWidget>
#include <QtCharts>

#include "../netatmo-w-analysis/backend/APIMonitor.h"
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"
#include "../netatmo-w-analysis/backend/MetricsAnalyzer.h"
#include "../netatmo-w-analysis/backend/NetatmoAPIHandler.h"
#include "../netatmo-w-analysis/backend/OldDataUploader.h"
#include "../netatmo-w-analysis/backend/RecentDataHandler.h"
#include "../netatmo-w-analysis/backend/WeatherAPIHandler.h"
#include "../netatmo-w-analysis/frontend/EphemerisPanel.h"
#include "../netatmo-w-analysis/frontend/HomePageChart.h"
#include "../netatmo-w-analysis/frontend/PredictionWidget/PredictionWidget.h"
#include "../netatmo-w-analysis/frontend/WeatherPredictionContainer.h"
#include "../netatmo-w-analysis/types/WeatherPrediction.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void buildWindow();
    void buildAPIHandlers();
    void buildLabels();
    void buildButtons();
    void buildCharts();
    void buildEphemerisPanel();
    void buildLayouts();
    void buildWeatherObjects();
    void createMenus();
    void createActions();

    QString measurementType();
    int durationInHours();
    void setBackgroundColor(const QColor &color);

public slots:
    void setAccessToken(QString);

    void updateCurrentExtTemperature(double currentTemperature);
    void updateMinExtTemperature(double minTemperature);
    void updateMaxExtTemperature(double maxTemperature);
    void updateMinExtTemperatureTime(long long timestamp);
    void updateMaxExtTemperatureTime(long long timestamp);
    void updateExtExtremeTemperaturesInfo();

    void updateCurrentIntTemperature(double currentTemperature);
    void updateMinIntTemperature(double minTemperature);
    void updateMaxIntTemperature(double maxTemperature);
    void updateMinIntTemperatureTime(int timestamp);
    void updateMaxIntTemperatureTime(int timestamp);

    void updateLastMeasurementDate(int timestamp);
    void updateActualisationDate(QDateTime timestamp);
    void updateRequestCounts();

    void updatePredictionWidgets(WeatherPrediction prediction);

    void addMonthData();
    void addMultipleMonthsData();

    void updateDailyIndoorDatabase();
    void updateDailyOutdoorDatabase();

    void exploreData();

    void displayMonthlyReport();
    void displayYearlyReport();

    void addDataFromCurrentMonths();
    void addDataFromLastDays();
    void changeChartsOptions();

    void showNormals();
    void showCredits();
    void showPredictionWindow();

    void postRecentDataRequests();

signals:
    void recentDataShouldBeUpdated();

protected:
    void paintEvent(QPaintEvent *event) override;

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

    // layouts
    QGridLayout *mainLayout;
    QHBoxLayout *predictionLayout;

    // API handlers
    NetatmoAPIHandler *apiHandler;
    RecentDataHandler *recentDataHandler;
    WeatherAPIHandler *weatherHandler;

    // database handlers
    DatabaseHandler *dbHandlerProd;
    DatabaseHandler *dbHandlerCopy;

    // weather objects
    PredictionWidget *predictionWidgets[4];
    WeatherPrediction weatherPrediction;
    WeatherPredictionContainer *weatherContainer;

    // other (provisional)
    QString accessToken = "";
    QLocale *deviceLocale;
    APIMonitor *apiMonitor;
    HomePageChart *indoorChart;
    HomePageChart *outdoorChart;
    MetricsAnalyzer *analyzer;

    // data uploader
    OldDataUploader *oldDataUploader;

    // actions
    QAction *requestCountsAction;
    QAction *addMonthDataAction;
    QAction *addMultipleMonthsDataAction;
    QAction *updateDailyIndoorDatabaseAction;
    QAction *updateDailyOutdoorDatabaseAction;
    QAction *exploreDataAction;
    QAction *displayMonthlyReportAction;
    QAction *displayYearlyReportAction;
    QAction *normalsAction;
    QAction *creditsAction;

    // chart options
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
    bool dataFromCurrentMonthsWasAdded = false;
    bool dataFromLastDaysWasAdded = false;
    QColor backgroundColor = QColor(Qt::white);

};

#endif // MAINWINDOW_H
