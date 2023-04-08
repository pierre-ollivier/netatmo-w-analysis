#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "backend/NetatmoAPIHandler.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    void buildWindow();
    void buildAPIHandler();
    void buildLabels();
    void buildButtons();
    void buildLayouts();

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

private:
    // labels
    QLabel *currentExtTempLabel, *currentIntTempLabel;
    QLabel *statusLabel;
    QLabel *currentMinExtTempLabel, *currentMaxExtTempLabel;
    QLabel *currentMinIntTempLabel, *currentMaxIntTempLabel;

    // buttons
    QPushButton *actualisationButton;

    //layouts
    QGridLayout *mainLayout;

    //other (provisional)
    NetatmoAPIHandler *apiHandler;

    //other (provisional)
    QString accessToken = "";
};

#endif // MAINWINDOW_H
