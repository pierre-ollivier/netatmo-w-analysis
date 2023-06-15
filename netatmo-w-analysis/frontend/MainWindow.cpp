#include "MainWindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "../netatmo-w-analysis/frontend/MonthlyReport.h"
#include "../netatmo-w-analysis/frontend/YearlyReport.h"
#include "../netatmo-w-analysis/frontend/NormalsVisualizer.h"
#include "../netatmo-w-analysis/backend/APIMonitor.h"
#include "../netatmo-w-analysis/backend/NormalComputer.h"
#include "../types/ExtTimestampRecord.h"

extern QString PATH_TO_PROD_DATABASE;
extern QString PATH_TO_COPY_DATABASE;

MainWindow::MainWindow()
{
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);
    menuBar = new QMenuBar();
    setMenuBar(menuBar);
    deviceLocale = new QLocale();
    apiMonitor = new APIMonitor();
    apiHandler = new NetatmoAPIHandler(apiMonitor, 20000);
    dbHandlerProd = new DatabaseHandler(PATH_TO_PROD_DATABASE);
    dbHandlerCopy = new DatabaseHandler(PATH_TO_COPY_DATABASE);
    oldDataUploader = new OldDataUploader(apiHandler);
    buildWindow();
}

void MainWindow::buildWindow() {
    buildAPIHandler();
    buildLabels();
    buildButtons();
    buildCharts();
    buildLayouts();
    createActions();
    createMenus();
}

void MainWindow::buildAPIHandler() { 
    apiHandler->postTokensRequest();
    connect(apiHandler, SIGNAL(accessTokenChanged(QString)),
            apiHandler, SLOT(postCurrentConditionsRequest(QString)));
    connect(apiHandler, SIGNAL(accessTokenChanged(QString)), SLOT(setAccessToken(QString)));
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
    connect(apiHandler, SIGNAL(extUTCTimeChanged(int)), SLOT(updateOutdoorChart()));
    connect(apiHandler, SIGNAL(intUTCTimeChanged(int)), SLOT(updateIndoorChart()));
}

void MainWindow::buildLabels() {
    statusLabel = new QLabel("Mesure : __/__/____ __:__:__\nActualisation : __/__/____ __:__:__");
    currentMinExtTempLabel = new QLabel("<font color=\"#0010ff\">↓</font> _,_ °C (__:__)");
    currentMaxExtTempLabel = new QLabel("<font color=\"#ff1000\">↑</font> _,_ °C (__:__)");
    currentMinExtTempLabel->setFont(QFont("Arial", 13));
    currentMaxExtTempLabel->setFont(QFont("Arial", 13));

    currentExtTempLabel = new QLabel("__,_<font color=\"#606060\"> °C</font>");
    currentExtTempLabel->setFont(QFont("Arial", 32));
    currentIntTempLabel = new QLabel("__,_<font color=\"#606060\"> °C</font>");
    currentIntTempLabel->setFont(QFont("Arial", 32));
    currentMinIntTempLabel = new QLabel("<font color=\"#0010ff\">↓</font> _,_ °C (__:__)");
    currentMaxIntTempLabel = new QLabel("<font color=\"#ff1000\">↑</font> _,_ °C (__:__)");
    currentMinIntTempLabel->setFont(QFont("Arial", 13));
    currentMaxIntTempLabel->setFont(QFont("Arial", 13));
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

void MainWindow::buildCharts() {
    indoorChart = new HomePageChart(apiHandler, "IndoorTimestampRecords", true);
    outdoorChart = new HomePageChart(apiHandler, "OutdoorTimestampRecords", false);

    h4Option = new QRadioButton("4 heures");
    h24Option = new QRadioButton("24 heures");
    h192Option = new QRadioButton("8 jours");
    h4Option->setChecked(true);
    QObject::connect(h4Option, SIGNAL(clicked(bool)), this, SLOT(changeChartsOptions()));
    QObject::connect(h24Option, SIGNAL(clicked(bool)), this, SLOT(changeChartsOptions()));
    QObject::connect(h192Option, SIGNAL(clicked(bool)), this, SLOT(changeChartsOptions()));

    temperatureOption = new QRadioButton("Température");
    humidityOption = new QRadioButton("Humidité");
    dewPointOption = new QRadioButton("Point de rosée");
    humidexOption = new QRadioButton("Humidex");
    temperatureOption->setChecked(true);
    QObject::connect(temperatureOption, SIGNAL(clicked(bool)), this, SLOT(changeChartsOptions()));
    QObject::connect(humidityOption, SIGNAL(clicked(bool)), this, SLOT(changeChartsOptions()));
    QObject::connect(dewPointOption, SIGNAL(clicked(bool)), this, SLOT(changeChartsOptions()));
    QObject::connect(humidexOption, SIGNAL(clicked(bool)), this, SLOT(changeChartsOptions()));
}

void MainWindow::buildLayouts() {
    chartsDurationOptionsLayout = new QHBoxLayout();
    chartsDurationOptionsLayout->addWidget(h4Option, 0, Qt::AlignCenter);
    chartsDurationOptionsLayout->addWidget(h24Option, 0, Qt::AlignCenter);
    chartsDurationOptionsLayout->addWidget(h192Option, 0, Qt::AlignCenter);

    chartsDurationOptionsGroupBox = new QGroupBox("");
    chartsDurationOptionsGroupBox->setLayout(chartsDurationOptionsLayout);

    chartsMeasurementOptionsLayout = new QHBoxLayout();
    chartsMeasurementOptionsLayout->addWidget(temperatureOption, 0, Qt::AlignCenter);
    chartsMeasurementOptionsLayout->addWidget(humidityOption, 0, Qt::AlignCenter);
    chartsMeasurementOptionsLayout->addWidget(dewPointOption, 0, Qt::AlignCenter);
    chartsMeasurementOptionsLayout->addWidget(humidexOption, 0, Qt::AlignCenter);

    chartsMeasurementOptionsGroupBox = new QGroupBox("");
    chartsMeasurementOptionsGroupBox->setLayout(chartsMeasurementOptionsLayout);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(currentExtTempLabel, 1, 0, 2, 1);
    mainLayout->addWidget(statusLabel, 0, 0);
//    mainLayout->addWidget(labelExtremes, 0, 1, 1, 2);
    mainLayout->addWidget(actualisationButton, 0, 3);
    mainLayout->addWidget(outdoorChart, 1, 1, 2, 2);
    mainLayout->addWidget(currentMaxExtTempLabel, 1, 3);
    mainLayout->addWidget(currentMinExtTempLabel, 2, 3);
    mainLayout->addWidget(currentIntTempLabel, 3, 0, 2, 1);
    mainLayout->addWidget(currentMaxIntTempLabel, 3, 3);
    mainLayout->addWidget(currentMinIntTempLabel, 4, 3);
    mainLayout->addWidget(indoorChart, 3, 1, 2, 2);
    mainLayout->addWidget(chartsDurationOptionsGroupBox, 5, 1, 1, 2);
    mainLayout->addWidget(chartsMeasurementOptionsGroupBox, 6, 1, 1, 2);

    // set window's layout
    mainWidget->setLayout(mainLayout);
}

void MainWindow::createActions() {
    requestCountsAction = new QAction("Rapport réseau...");
    connect(requestCountsAction, SIGNAL(triggered()), SLOT(updateRequestCounts()));

    addMonthDataAction = new QAction("Ajouter des données mensuelles...");
    connect(addMonthDataAction, SIGNAL(triggered()), SLOT(addMonthData()));
    addMultipleMonthsDataAction = new QAction("Ajouter des données mensuelles sur plusieurs mois...");
    connect(addMultipleMonthsDataAction, SIGNAL(triggered()), SLOT(addMultipleMonthsData()));
    updateDailyIndoorDatabaseAction = new QAction("Mettre à jour la base de données quotidiennes intérieures");
    connect(updateDailyIndoorDatabaseAction, SIGNAL(triggered()), SLOT(updateDailyIndoorDatabase()));
    updateDailyOutdoorDatabaseAction = new QAction("Mettre à jour la base de données quotidiennes extérieures");
    connect(updateDailyOutdoorDatabaseAction, SIGNAL(triggered()), SLOT(updateDailyOutdoorDatabase()));

    displayMonthlyReportAction = new QAction("Rapport mensuel");
    connect(displayMonthlyReportAction, SIGNAL(triggered()), SLOT(displayMonthlyReport()));
    displayYearlyReportAction = new QAction("Climatologie générale");
    connect(displayYearlyReportAction, SIGNAL(triggered()), SLOT(displayYearlyReport()));
    normalsAction = new QAction("Normales");
    connect(normalsAction, SIGNAL(triggered()), SLOT(showNormals()));
}

void MainWindow::createMenus() {
    QMenu *networkMenu = menuBar->addMenu(tr("&Réseau"));
    networkMenu->addAction(requestCountsAction);
    QMenu *dataMenu = menuBar->addMenu(tr("Données"));
    dataMenu->addAction(addMonthDataAction);
    dataMenu->addAction(addMultipleMonthsDataAction);
    dataMenu->addAction(updateDailyIndoorDatabaseAction);
    dataMenu->addAction(updateDailyOutdoorDatabaseAction);
    QMenu *climatologyMenu = menuBar->addMenu(tr("&Climatologie"));
    climatologyMenu->addAction(displayMonthlyReportAction);
    climatologyMenu->addAction(displayYearlyReportAction);
    climatologyMenu->addAction(normalsAction);
}

void MainWindow::setAccessToken(QString newAccessToken) {
    QTimer::singleShot(170 * 60 * 1000, apiHandler, SLOT(postRefreshTokenRequest()));
    accessToken = newAccessToken;
    oldDataUploader->setAccessToken(accessToken);
    addDataFromCurrentMonths();
    updateIndoorChart();
    updateOutdoorChart();
}

void MainWindow::updateIndoorChart(QString measurementType, int durationInHours) {
    if (measurementType == "") measurementType = _measurementType;
    if (durationInHours == 0) durationInHours = _durationInHours;
    if (accessToken != "") indoorChart->gatherChartData(accessToken, measurementType, true, durationInHours);
}

void MainWindow::updateOutdoorChart(QString measurementType, int durationInHours) {
    if (measurementType == "") measurementType = _measurementType;
    if (durationInHours == 0) durationInHours = _durationInHours;
    if (accessToken != "") outdoorChart->gatherChartData(accessToken, measurementType, false, durationInHours);
}

void MainWindow::addDataFromCurrentMonths() {
    QDate lastAddedOutdoorDate = dbHandlerProd->getLatestDateTimeFromDatabase("OutdoorDailyRecords").date();
    QDate lastAddedIndoorDate = dbHandlerProd->getLatestDateTimeFromDatabase("IndoorDailyRecords").date();

    oldDataUploader->addDataFromCurrentMonths(lastAddedOutdoorDate.addDays(1),
                                              QDate::currentDate(), false);
    oldDataUploader->addDataFromCurrentMonths(lastAddedIndoorDate.addDays(1),
                                              QDate::currentDate(), true);
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
    QMessageBox::information(this, "Rapport réseau",
                             "Requêtes restantes : <br><b>"
                             + QString::number(remainingRequests10s) + "</b> / 10 secondes<br>"
                             + "<b>" + QString::number(remainingRequests1h) + "</b> / 1 heure");
}

void MainWindow::addMonthData() {
    QString fileName = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", "D:/Mes programmes/RegressionTemperature/Données Netatmo", "*.csv");
    QString q = "Confirmer la saisie ? \n\n";
    q += "Nom du fichier : " + fileName.mid(56) + "\n";

    bool isIndoorData = (fileName.size() > 7 && fileName[fileName.size() - 12] == 'C');

    if (isIndoorData) {
        q += "Dernière date dans la base de données intérieures : ";
        q += dbHandlerProd->getLatestDateTimeFromDatabase("IndoorTimestampRecords").date().toString("dd/MM/yyyy");
    }
    else {
        q += "Dernière date dans la base de données extérieures : ";
        q += dbHandlerProd->getLatestDateTimeFromDatabase("OutdoorTimestampRecords").date().toString("dd/MM/yyyy");
    }

    int response = QMessageBox::question(this, "Confirmation", q, QMessageBox ::Yes | QMessageBox::No);

    if (response == QMessageBox::Yes) {
        if (isIndoorData) {
            dbHandlerProd->postFromIndoorCsv(fileName, "IndoorTimestampRecords");
        }
        else {
            dbHandlerProd->postFromOutdoorCsv(fileName, "OutdoorTimestampRecords");
        }
    }
    else if (response == QMessageBox::No) QMessageBox::warning(this, "Annulation", "Opération annulée.");

}

void MainWindow::addMultipleMonthsData() {
    bool okBegin = false, okEnd = false;
    QString beginDate = QInputDialog::getText(
                this, "Mois de début", "Mois de début (au format MM/AAAA) :", QLineEdit::Normal, QString(), &okBegin);
    if (!okBegin) return;
    QString endDate = QInputDialog::getText(
                this, "Mois de fin", "Mois de fin (au format MM/AAAA) :", QLineEdit::Normal, QString(), &okEnd);
    if (!okEnd) return;

    int indoorOrOutdoor = QMessageBox::question(this, "Lieu", "Considérer les données <b> intérieures </b> ?",
                                                QMessageBox::Yes, QMessageBox::No);

    bool isIndoorData = indoorOrOutdoor == QMessageBox::Yes;

    QString q = "Confirmer la saisie ? \n\n";
    q += "Mois de début : " + beginDate + "\n";
    q += "Mois de fin : " + endDate + "\n";
    q += "Données : ";
    q += (isIndoorData ? "intérieures" : "extérieures");

    int response = QMessageBox::question(this, "Confirmation", q, QMessageBox ::Yes | QMessageBox::No);

    if (response == QMessageBox::Yes) {
        if (isIndoorData) {
            dbHandlerProd->postFromMultipleIndoorCsv(
                        "D:/Mes programmes/RegressionTemperature/Données Netatmo/Intérieur",
                        "IndoorTimestampRecords",
                        beginDate,
                        endDate);
        }
        else {
            dbHandlerProd->postFromMultipleOutdoorCsv(
                        "D:/Mes programmes/RegressionTemperature/Données Netatmo",
                        "OutdoorTimestampRecords",
                        beginDate,
                        endDate);
        }
    }
    else if (response == QMessageBox::No) QMessageBox::warning(this, "Annulation", "Opération annulée.");

}

void MainWindow::updateDailyIndoorDatabase() {
    bool okBegin = false, okEnd = false;
    QString beginDate = QInputDialog::getText(
                this, "Date de début", "Date de début (au format JJ/MM/AAAA) :", QLineEdit::Normal, QString(), &okBegin);
    if (!okBegin) return;
    QString endDate = QInputDialog::getText(
                this, "Date de fin", "Date de fin (au format JJ/MM/AAAA) :", QLineEdit::Normal, QString(), &okEnd);
    if (!okEnd) return;

    QString q = "Confirmer la saisie ? \n\n";
    q += "Date de début : " + beginDate + "\n";
    q += "Date de fin : " + endDate;
    q += "Dernière date dans la base de données intérieures : ";
    q += dbHandlerProd->getLatestDateTimeFromDatabase("IndoorDailyRecords").date().toString("dd/MM/yyyy");

    int response = QMessageBox::question(this, "Confirmation", q, QMessageBox ::Yes | QMessageBox::No);

    if (response == QMessageBox::Yes) {
        dbHandlerProd->updateIndoorDailyRecords(
                    QDate::fromString(beginDate, "dd/MM/yyyy"),
                    QDate::fromString(endDate, "dd/MM/yyyy"));
    }

    else if (response == QMessageBox::No) QMessageBox::warning(this, "Annulation", "Opération annulée.");
}

void MainWindow::updateDailyOutdoorDatabase() {
    bool okBegin = false, okEnd = false;
    QString beginDate = QInputDialog::getText(
                this, "Date de début", "Date de début (au format JJ/MM/AAAA) :", QLineEdit::Normal, QString(), &okBegin);
    if (!okBegin) return;
    QString endDate = QInputDialog::getText(
                this, "Date de fin", "Date de fin (au format JJ/MM/AAAA) :", QLineEdit::Normal, QString(), &okEnd);
    if (!okEnd) return;

    QString q = "Confirmer la saisie ? \n\n";
    q += "Date de début : " + beginDate + "\n";
    q += "Date de fin : " + endDate;
    q += "Dernière date dans la base de données extérieures : ";
    q += dbHandlerProd->getLatestDateTimeFromDatabase("OutdoorDailyRecords").date().toString("dd/MM/yyyy");

    int response = QMessageBox::question(this, "Confirmation", q, QMessageBox ::Yes | QMessageBox::No);

    if (response == QMessageBox::Yes) {
        dbHandlerProd->updateOutdoorDailyRecords(
                    QDate::fromString(beginDate, "dd/MM/yyyy"),
                    QDate::fromString(endDate, "dd/MM/yyyy"));
    }

    else if (response == QMessageBox::No) QMessageBox::warning(this, "Annulation", "Opération annulée.");
}

void MainWindow::displayMonthlyReport() {
    MonthlyReport *report = new MonthlyReport();
    report->show();
}

void MainWindow::displayYearlyReport() {
    YearlyReport *report = new YearlyReport();
    report->show();
}

void MainWindow::changeChartsOptions() {
    if (temperatureOption->isChecked()) _measurementType = "temperature";
    if (humidityOption->isChecked()) _measurementType = "humidity";
    if (dewPointOption->isChecked()) _measurementType = "dewPoint";
    if (humidexOption->isChecked()) _measurementType = "humidex";
    if (h4Option->isChecked()) _durationInHours = 4;
    if (h24Option->isChecked()) _durationInHours = 24;
    if (h192Option->isChecked()) _durationInHours = 192;
    updateOutdoorChart();
    updateIndoorChart();
}

void MainWindow::showNormals() {
    NormalComputer computer = NormalComputer(dbHandlerProd);

    NormalsVisualizer *visualizer = new NormalsVisualizer(&computer);
    visualizer->show();
}
