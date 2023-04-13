#include "MainWindow.h"
#include <QDateTime>
#include "backend/APIMonitor.h"
#include <QTimer>

MainWindow::MainWindow()
{
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);
    deviceLocale = new QLocale();
    apiMonitor = new APIMonitor();
    buildWindow();
    QTimer *testMonitorTimer = new QTimer();
    testMonitorTimer->start(500);
    connect(testMonitorTimer, SIGNAL(timeout()), this, SLOT(updateRequestCounts()));
}

void MainWindow::buildWindow() {
    buildAPIHandler();
    buildLabels();
    buildButtons();
    buildLayouts();
}

void MainWindow::buildAPIHandler() {
    apiHandler = new NetatmoAPIHandler(apiMonitor, 20000);
    apiHandler->postTokensRequest();
    connect(apiHandler, SIGNAL(accessTokenChanged(QString)),
            apiHandler, SLOT(postCurrentConditionsRequest(QString)));
    connect(apiHandler, SIGNAL(extTemperatureChanged(double)), this, SLOT(updateCurrentExtTemperature(double)));
    connect(apiHandler, SIGNAL(intTemperatureChanged(double)), this, SLOT(updateCurrentIntTemperature(double)));
    connect(apiHandler, SIGNAL(extUTCTimeChanged(int)), this, SLOT(updateLastMeasurementDate(int)));
    connect(apiHandler, SIGNAL(currentTimeChanged(QDateTime)), this, SLOT(updateActualisationDate(QDateTime)));
    connect(apiHandler, SIGNAL(extMinTemperatureChanged(double)), this, SLOT(updateMinExtTemperature(double)));
    connect(apiHandler, SIGNAL(extMaxTemperatureChanged(double)), this, SLOT(updateMaxExtTemperature(double)));
    connect(apiHandler, SIGNAL(extMinTemperatureTimeChanged(int)),
            this, SLOT(updateMinExtTemperatureTime(int)));
    connect(apiHandler, SIGNAL(extMaxTemperatureTimeChanged(int)),
            this, SLOT(updateMaxExtTemperatureTime(int)));
    connect(apiHandler, SIGNAL(intMinTemperatureChanged(double)), this, SLOT(updateMinIntTemperature(double)));
    connect(apiHandler, SIGNAL(intMaxTemperatureChanged(double)), this, SLOT(updateMaxIntTemperature(double)));
    connect(apiHandler, SIGNAL(intMinTemperatureTimeChanged(int)),
            this, SLOT(updateMinIntTemperatureTime(int)));
    connect(apiHandler, SIGNAL(intMaxTemperatureTimeChanged(int)),
            this, SLOT(updateMaxIntTemperatureTime(int)));
}

void MainWindow::buildLabels() {
    statusLabel = new QLabel("Mesure : --/--/---- --:--:--\nActualisation : --/--/---- --:--:--");
    currentMinExtTempLabel = new QLabel("<font color=\"#0010ff\">↓</font> -,- °C (--:--)");
    currentMaxExtTempLabel = new QLabel("<font color=\"#ff1000\">↑</font> -,- °C (--:--)");
    currentMinExtTempLabel->setFont(QFont("Arial", 13));
    currentMaxExtTempLabel->setFont(QFont("Arial", 13));

    currentExtTempLabel = new QLabel();
    currentExtTempLabel->setFont(QFont("Arial", 32));
    currentIntTempLabel = new QLabel();
    currentIntTempLabel->setFont(QFont("Arial", 32));
    currentMinIntTempLabel = new QLabel("<font color=\"#0010ff\">↓</font> -,- °C (--:--)");
    currentMaxIntTempLabel = new QLabel("<font color=\"#ff1000\">↑</font> -,- °C (--:--)");
    currentMinIntTempLabel->setFont(QFont("Arial", 13));
    currentMaxIntTempLabel->setFont(QFont("Arial", 13));
    currentRequestStatus = new QLabel("Requêtes restantes : 50 / 10 secondes, 500 / 1 heure");
}

void MainWindow::buildButtons() {
    actualisationButton = new QPushButton("Actualiser");
    actualisationButton->setFont(QFont("Verdana", 14));
    actualisationButton->setIcon(QIcon(
        "D:/Mes programmes/netatmo-w-analysis/images/Image actualisation.png"
        ));
    connect(actualisationButton, SIGNAL(pressed()),
            apiHandler, SLOT(postCurrentConditionsRequest()));
}

void MainWindow::buildLayouts() {
    mainLayout = new QGridLayout();
    mainLayout->addWidget(currentExtTempLabel, 1, 0, 2, 1);
    mainLayout->addWidget(statusLabel, 0, 0);
//    mainLayout->addWidget(labelExtremes, 0, 1, 1, 2);
    mainLayout->addWidget(actualisationButton, 0, 3);
//    mainLayout->addWidget(vuegp, 1, 1, 2, 2);
    mainLayout->addWidget(currentMaxExtTempLabel, 1, 3);
    mainLayout->addWidget(currentMinExtTempLabel, 2, 3);
    mainLayout->addWidget(currentIntTempLabel, 3, 0, 2, 1);
    mainLayout->addWidget(currentMaxIntTempLabel, 3, 3);
    mainLayout->addWidget(currentMinIntTempLabel, 4, 3);
//    mainLayout->addWidget(vuegpint, 3, 1, 2, 2);
    mainLayout->addWidget(currentRequestStatus, 5, 0, 1, 4);

    // set window's layout
    mainWidget->setLayout(mainLayout);
}

void MainWindow::updateCurrentExtTemperature(double currentTemperature) {
    currentExtTempLabel->setText(deviceLocale->toString(currentTemperature, 'f', 1)
                                 + "<font color=\"#606060\"> °C</font>");
}

void MainWindow::updateCurrentIntTemperature(double currentTemperature) {
    currentIntTempLabel->setText(deviceLocale->toString(currentTemperature, 'f', 1)
                                 + "<font color=\"#606060\"> °C</font>");
}

void MainWindow::updateLastMeasurementDate(int timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    statusLabel->setText(statusLabel->text().replace(9, 19, dt.toString("dd/MM/yyyy hh:mm:ss")));
}

void MainWindow::updateActualisationDate(QDateTime timestamp) {
    statusLabel->setText(statusLabel->text().replace(45, 19, timestamp.toString("dd/MM/yyyy hh:mm:ss")));
}

void MainWindow::updateMinExtTemperature(double minTemperature) {
    const int lenToReplace = currentMinExtTempLabel->text().length() - 42;
    currentMinExtTempLabel->setText(currentMinExtTempLabel->text().replace(31,  // the blue arrow and the HTML tags take space
                                                           lenToReplace,
                                                           deviceLocale->toString(minTemperature, 'f', 1)));
}

void MainWindow::updateMaxExtTemperature(double maxTemperature) {
    const int lenToReplace = currentMaxExtTempLabel->text().length() - 42;
    currentMaxExtTempLabel->setText(currentMaxExtTempLabel->text().replace(31,  // the red arrow and the HTML tags take space
                                                           lenToReplace,
                                                           deviceLocale->toString(maxTemperature, 'f', 1)));
}

void MainWindow::updateMinExtTemperatureTime(int timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    const int positionToReplace = currentMinExtTempLabel->text().length() - 7;
    currentMinExtTempLabel->setText(currentMinExtTempLabel->text().replace(positionToReplace,
                                                           7,
                                                           dt.toString("(hh:mm)")));
}

void MainWindow::updateMaxExtTemperatureTime(int timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    const int positionToReplace = currentMaxExtTempLabel->text().length() - 7;
    currentMaxExtTempLabel->setText(currentMaxExtTempLabel->text().replace(positionToReplace,
                                                           7,
                                                           dt.toString("(hh:mm)")));
}

void MainWindow::updateMinIntTemperature(double minTemperature) {
    const int lenToReplace = currentMinIntTempLabel->text().length() - 42;
    currentMinIntTempLabel->setText(currentMinIntTempLabel->text().replace(31,  // the blue arrow and the HTML tags take space
                                                           lenToReplace,
                                                           deviceLocale->toString(minTemperature, 'f', 1)));
}

void MainWindow::updateMaxIntTemperature(double maxTemperature) {
    const int lenToReplace = currentMaxIntTempLabel->text().length() - 42;
    currentMaxIntTempLabel->setText(currentMaxIntTempLabel->text().replace(31,  // the red arrow and the HTML tags take space
                                                           lenToReplace,
                                                           deviceLocale->toString(maxTemperature, 'f', 1)));
}

void MainWindow::updateMinIntTemperatureTime(int timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    const int positionToReplace = currentMinIntTempLabel->text().length() - 7;
    currentMinIntTempLabel->setText(currentMinIntTempLabel->text().replace(positionToReplace,
                                                           7,
                                                           dt.toString("(hh:mm)")));
}

void MainWindow::updateMaxIntTemperatureTime(int timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    const int positionToReplace = currentMaxIntTempLabel->text().length() - 7;
    currentMaxIntTempLabel->setText(currentMaxIntTempLabel->text().replace(positionToReplace,
                                                           7,
                                                           dt.toString("(hh:mm)")));
}

void MainWindow::updateRequestCounts() {
    int remainingRequests10s = 50 - apiMonitor->requestsCountLast10s();
    int remainingRequests1h = 500 - apiMonitor->requestsCountLasth();
    currentRequestStatus->setText("Requêtes restantes : " + QString::number(remainingRequests10s)
                                  + " / 10 secondes, " + QString::number(remainingRequests1h)
                                  + " / 1 heure");
}
