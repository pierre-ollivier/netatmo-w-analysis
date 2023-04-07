#include "MainWindow.h"
#include <QDateTime>

MainWindow::MainWindow()
{
    buildWindow();
    // The following lines are tests
    apiHandler = new NetatmoAPIHandler();
    apiHandler->postTokensRequest();
//    apiHandler->postCurrentConditionsRequest();
    connect(apiHandler, SIGNAL(accessTokenChanged(QString)), this, SLOT(logToken(QString)));
    connect(apiHandler, SIGNAL(refreshTokenChanged(QString)), this, SLOT(logToken(QString)));
    connect(apiHandler, SIGNAL(accessTokenChanged(QString)),
            apiHandler, SLOT(postCurrentConditionsRequest(QString)));
    connect(apiHandler, SIGNAL(extTemperatureChanged(double)), this, SLOT(updateCurrentExtTemperature(double)));
    connect(apiHandler, SIGNAL(intTemperatureChanged(double)), this, SLOT(updateCurrentIntTemperature(double)));
    connect(apiHandler, SIGNAL(extUTCTimeChanged(int)), this, SLOT(updateLastMeasurementDate(int)));
    connect(apiHandler, SIGNAL(currentTimeChanged(QDateTime)), this, SLOT(updateActualisationDate(QDateTime)));

}

void MainWindow::buildWindow() {

    buildLabels();
    buildButtons();
    buildLayouts();

}

void MainWindow::buildLabels() {
    labelStatus = new QLabel("Mesure : --/--/---- --:--:--\nActualisation : --/--/---- --:--:--");
    labelDailyTmin = new QLabel("<font color=\"#0010ff\">↓</font> -,- °C (--:--)");
    labelDailyTmax = new QLabel("<font color=\"#ff1000\">↑</font> -,- °C (--:--)");
    labelDailyTmin->setFont(QFont("Arial", 13));
    labelDailyTmax->setFont(QFont("Arial", 13));

    labelCurrentTempExt = new QLabel();
    labelCurrentTempExt->setFont(QFont("Arial", 32));
    labelCurrentTempInt = new QLabel();
    labelCurrentTempInt->setFont(QFont("Arial", 32));
    labelDailyTminInt = new QLabel("<font color=\"#0010ff\">↓</font> -,- °C (--:--)");
    labelDailyTmaxInt = new QLabel("<font color=\"#ff1000\">↑</font> -,- °C (--:--)");
    labelDailyTminInt->setFont(QFont("Arial", 13));
    labelDailyTmaxInt->setFont(QFont("Arial", 13));
}

void MainWindow::buildButtons() {
    actualisationButton = new QPushButton("Actualiser");
    actualisationButton->setFont(QFont("Verdana", 14));
//    boutonActualisation->setIcon()
}

void MainWindow::buildLayouts() {
    mainLayout = new QGridLayout();
    mainLayout->addWidget(labelCurrentTempExt, 1, 0, 2, 1);
    mainLayout->addWidget(labelStatus, 0, 0);
//    mainLayout->addWidget(labelExtremes, 0, 1, 1, 2);
    mainLayout->addWidget(actualisationButton, 0, 3);
//    mainLayout->addWidget(vuegp, 1, 1, 2, 2);
    mainLayout->addWidget(labelDailyTmax, 1, 3);
    mainLayout->addWidget(labelDailyTmin, 2, 3);
    mainLayout->addWidget(labelCurrentTempInt, 3, 0, 2, 1);
    mainLayout->addWidget(labelDailyTmaxInt, 3, 3);
    mainLayout->addWidget(labelDailyTminInt, 4, 3);
//    mainLayout->addWidget(vuegpint, 3, 1, 2, 2);

    // set window's layout
    setLayout(mainLayout);
}

void MainWindow::logToken(QString token) {
    qDebug() << "TOKEN: " << token;
}

void MainWindow::updateCurrentExtTemperature(double currentTemperature) {
    labelCurrentTempExt->setText(QString::number(currentTemperature) + "<font color=\"#606060\"> °C</font>");
    qDebug() << "EXT TEMPERATURE: " << currentTemperature;
}

void MainWindow::updateCurrentIntTemperature(double currentTemperature) {
    labelCurrentTempInt->setText(QString::number(currentTemperature) + "<font color=\"#606060\"> °C</font>");
    qDebug() << "INT TEMPERATURE: " << currentTemperature;
}

void MainWindow::updateLastMeasurementDate(int timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    labelStatus->setText(labelStatus->text().replace(9, 19, dt.toString("dd/MM/yyyy hh:mm:ss")));
}

void MainWindow::updateActualisationDate(QDateTime timestamp) {
    labelStatus->setText(labelStatus->text().replace(45, 19, timestamp.toString("dd/MM/yyyy hh:mm:ss")));
}
