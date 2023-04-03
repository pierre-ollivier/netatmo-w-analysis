#include "MainWindow.h"

MainWindow::MainWindow()
{
    buildWindow();
    // The two following lines are tests
    apiHandler = new NetatmoAPIHandler();
    apiHandler->sendTokensRequest();

}

void MainWindow::buildWindow() {

    buildLabels();
    buildLayouts();

}

void MainWindow::buildLabels() {
    labelStatus = new QLabel("Mesure : --/--/---- --:--:--\nActualisation : --/--/---- --:--:--");
    labelDailyTmin = new QLabel("<font color=\"#0010ff\">↓</font> -,- °C (--:--)");
    labelDailyTmax = new QLabel("<font color=\"#ff1000\">↑</font> -,- °C (--:--)");
    labelDailyTmin->setFont(QFont("Arial", 13));
    labelDailyTmax->setFont(QFont("Arial", 13));

    labelCurrentTemp = new QLabel();
    labelCurrentTemp->setFont(QFont("Arial", 32));
    labelCurrentTempInt = new QLabel();
    labelCurrentTempInt->setFont(QFont("Arial", 32));
    labelDailyTminInt = new QLabel("<font color=\"#0010ff\">↓</font> -,- °C (--:--)");
    labelDailyTmaxInt = new QLabel("<font color=\"#ff1000\">↑</font> -,- °C (--:--)");
    labelDailyTminInt->setFont(QFont("Arial", 13));
    labelDailyTmaxInt->setFont(QFont("Arial", 13));
}

void MainWindow::buildLayouts() {
    mainLayout = new QGridLayout();
    mainLayout->addWidget(labelCurrentTemp, 1, 0, 2, 1);
    mainLayout->addWidget(labelStatus, 0, 0);
//    mainLayout->addWidget(labelExtremes, 0, 1, 1, 2);
//    mainLayout->addWidget(boutonActualisation, 0, 3);
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
