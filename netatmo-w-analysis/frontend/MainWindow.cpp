#include "MainWindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>

#include "../netatmo-w-analysis/backend/NormalComputer.h"
#include "../netatmo-w-analysis/frontend/DataExplorator.h"
#include "../netatmo-w-analysis/frontend/MonthlyReport.h"
#include "../netatmo-w-analysis/frontend/NormalsVisualizer.h"
#include "../netatmo-w-analysis/frontend/PredictionWindow.h"
#include "../netatmo-w-analysis/frontend/YearlyReport.h"
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
    analyzer = new MetricsAnalyzer();
    apiHandler = new NetatmoAPIHandler(apiMonitor, 20000);
    recentDataHandler = new RecentDataHandler(apiMonitor);
    dbHandlerProd = new DatabaseHandler(PATH_TO_PROD_DATABASE);
    dbHandlerCopy = new DatabaseHandler(PATH_TO_COPY_DATABASE);
    oldDataUploader = new OldDataUploader(apiHandler);
    connect(this, SIGNAL(recentDataShouldBeUpdated()), SLOT(postRecentDataRequests()));
    buildWindow();

    weatherHandler = new WeatherAPIHandler();
    weatherHandler->postWeatherRequest();
    connect(weatherHandler, SIGNAL(predictionDataRetrieved(WeatherPrediction)), SLOT(updatePredictionWidgets(WeatherPrediction)));
}

void MainWindow::buildWindow() {
    buildCharts();
    buildAPIHandlers();
    buildLabels();
    buildButtons();
    buildEphemerisPanel();
    buildWeatherObjects();
    buildLayouts();
    createActions();
    createMenus();
    setBackgroundColor(QColor(219, 197, 236));
}

void MainWindow::buildAPIHandlers() {
    apiHandler->postTokensRequest();
    connect(apiHandler, SIGNAL(accessTokenChanged(QString)),
            apiHandler, SLOT(postCurrentConditionsRequest(QString)));
    connect(apiHandler, SIGNAL(accessTokenChanged(QString)), SLOT(setAccessToken(QString)));
    connect(apiHandler, SIGNAL(extTemperatureChanged(double)), this, SLOT(updateCurrentExtTemperature(double)));
    connect(apiHandler, SIGNAL(intTemperatureChanged(double)), this, SLOT(updateCurrentIntTemperature(double)));
    connect(apiHandler, SIGNAL(extUTCTimeChanged(int)), this, SLOT(updateLastMeasurementDate(int)));
    connect(apiHandler, SIGNAL(currentTimeChanged(QDateTime)), this, SLOT(updateActualisationDate(QDateTime)));
    connect(apiHandler, SIGNAL(intMinTemperatureChanged(double)), this, SLOT(updateMinIntTemperature(double)));
    connect(apiHandler, SIGNAL(intMaxTemperatureChanged(double)), this, SLOT(updateMaxIntTemperature(double)));
    connect(apiHandler, SIGNAL(intMinTemperatureTimeChanged(int)),
            this, SLOT(updateMinIntTemperatureTime(int)));
    connect(apiHandler, SIGNAL(intMaxTemperatureTimeChanged(int)),
            this, SLOT(updateMaxIntTemperatureTime(int)));

    connect(apiHandler, SIGNAL(extUTCTimeChanged(int)), SIGNAL(recentDataShouldBeUpdated()));
    connect(apiHandler, SIGNAL(intUTCTimeChanged(int)), SIGNAL(recentDataShouldBeUpdated()));

    connect(recentDataHandler, SIGNAL(recentOutdoorRecordListRetrieved(QList<ExtTimestampRecord>)),
            oldDataUploader, SLOT(logOutdoorTimestampRecords(QList<ExtTimestampRecord>)));
    connect(recentDataHandler, SIGNAL(recentIndoorRecordListRetrieved(QList<IntTimestampRecord>)),
            oldDataUploader, SLOT(logIndoorTimestampRecords(QList<IntTimestampRecord>)));

    connect(recentDataHandler, SIGNAL(recentOutdoorRecordListRetrieved(QList<ExtTimestampRecord>)),
            SLOT(updateExtExtremeTemperaturesInfo()));

    connect(recentDataHandler, SIGNAL(outdoorRecordListRetrieved(QList<ExtTimestampRecord>)),
            outdoorChart, SLOT(drawChart(QList<ExtTimestampRecord>)));
    connect(recentDataHandler, SIGNAL(indoorRecordListRetrieved(QList<IntTimestampRecord>)),
            indoorChart, SLOT(drawChart(QList<IntTimestampRecord>)));
}

void MainWindow::buildLabels() {
    statusLabel = new QLabel("Mesure : __/__/____ __:__:__\nActualisation : __/__/____ __:__:__");
    currentMinExtTempLabel = new QLabel("<font color=\"#0010ff\">↓</font> __,_ °C (__:__)");
    currentMaxExtTempLabel = new QLabel("<font color=\"#ff1000\">↑</font> __,_ °C (__:__)");
    currentMinExtTempLabel->setFont(QFont("Arial", 13));
    currentMaxExtTempLabel->setFont(QFont("Arial", 13));

    currentExtTempLabel = new QLabel("__,_<font color=\"#606060\"> °C</font>");
    currentExtTempLabel->setFont(QFont("Arial", 32));
    currentIntTempLabel = new QLabel("__,_<font color=\"#606060\"> °C</font>");
    currentIntTempLabel->setFont(QFont("Arial", 32));
    currentMinIntTempLabel = new QLabel("<font color=\"#0010ff\">↓</font> __,_ °C (__:__)");
    currentMaxIntTempLabel = new QLabel("<font color=\"#ff1000\">↑</font> __,_ °C (__:__)");
    currentMinIntTempLabel->setFont(QFont("Arial", 13));
    currentMaxIntTempLabel->setFont(QFont("Arial", 13));
}

void MainWindow::buildButtons() {
    actualisationButton = new QPushButton("Actualiser");
    actualisationButton->setFont(QFont("Verdana", 14));
    actualisationButton->setIcon(QIcon(
        QCoreApplication::applicationDirPath()
            + "/../../netatmo-w-analysis/netatmo-w-analysis/images/Image actualisation.png"
        ));
    connect(actualisationButton, SIGNAL(pressed()),
            apiHandler, SLOT(postCurrentConditionsRequest()));
}

void MainWindow::buildCharts() {
    indoorChart = new HomePageChart("IndoorTimestampRecords", true);
    outdoorChart = new HomePageChart("OutdoorTimestampRecords", false);

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

void MainWindow::buildEphemerisPanel() {
    ephemerisPanel = new EphemerisPanel();
    connect(oldDataUploader, SIGNAL(outdoorTimestampRecordsLogged()), ephemerisPanel, SLOT(updateStdevLabel()));
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

    predictionLayout = new QHBoxLayout();
    for (int i = 0;i < 4; i++) {
        predictionLayout->addWidget(predictionWidgets[i]);
    }

    weatherContainer->setLayout(predictionLayout);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(currentExtTempLabel, 1, 0, 2, 1);
    mainLayout->addWidget(statusLabel, 0, 0);
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
    mainLayout->addWidget(ephemerisPanel, 0, 4, 5, 1);
    mainLayout->addWidget(weatherContainer, 5, 4, 2, 1);

    // set window's layout
    mainWidget->setLayout(mainLayout);
}

void MainWindow::buildWeatherObjects() {
    weatherPrediction = WeatherPrediction();
    weatherContainer = new WeatherPredictionContainer();
    for (int i = 0; i < 4; i++) {
        predictionWidgets[i] = new PredictionWidget();
    }
    connect(weatherContainer, SIGNAL(clicked()), SLOT(showPredictionWindow()));
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

    exploreDataAction = new QAction("Explorer les données");
    connect(exploreDataAction, SIGNAL(triggered()), SLOT(exploreData()));

    displayMonthlyReportAction = new QAction("Rapport mensuel");
    connect(displayMonthlyReportAction, SIGNAL(triggered()), SLOT(displayMonthlyReport()));
    displayYearlyReportAction = new QAction("Climatologie générale");
    connect(displayYearlyReportAction, SIGNAL(triggered()), SLOT(displayYearlyReport()));

    normalsAction = new QAction("Normales");
    connect(normalsAction, SIGNAL(triggered()), SLOT(showNormals()));

    creditsAction = new QAction("Crédits");
    connect(creditsAction, SIGNAL(triggered()), SLOT(showCredits()));
}

void MainWindow::createMenus() {
    QMenu *networkMenu = menuBar->addMenu(tr("&Réseau"));
    networkMenu->addAction(requestCountsAction);
    QMenu *handleDataMenu = menuBar->addMenu(tr("&Gestion des données"));
    handleDataMenu->addAction(addMonthDataAction);
    handleDataMenu->addAction(addMultipleMonthsDataAction);
    handleDataMenu->addAction(updateDailyIndoorDatabaseAction);
    handleDataMenu->addAction(updateDailyOutdoorDatabaseAction);
    QMenu *exploreDataMenu = menuBar->addMenu(tr("&Exploration des données"));
    exploreDataMenu->addAction(exploreDataAction);
    QMenu *climatologyMenu = menuBar->addMenu(tr("&Climatologie"));
    climatologyMenu->addAction(displayMonthlyReportAction);
    climatologyMenu->addAction(displayYearlyReportAction);
    climatologyMenu->addAction(normalsAction);
    QMenu *creditsMenu = menuBar->addMenu(tr("C&rédits"));
    creditsMenu->addAction(creditsAction);
}

void MainWindow::setAccessToken(QString newAccessToken) {
    QTimer::singleShot(170 * 60 * 1000, apiHandler, SLOT(postRefreshTokenRequest()));
    accessToken = newAccessToken;
    oldDataUploader->setAccessToken(accessToken);
    if (!dataFromCurrentMonthsWasAdded) addDataFromCurrentMonths();
    if (!dataFromLastDaysWasAdded) addDataFromLastDays();
}

void MainWindow::addDataFromCurrentMonths() {
    QDate lastAddedOutdoorDate = dbHandlerProd->getLatestDateTimeFromDatabase("OutdoorDailyRecords").date();
    QDate lastAddedIndoorDate = dbHandlerProd->getLatestDateTimeFromDatabase("IndoorDailyRecords").date();

    oldDataUploader->addDataFromCurrentMonths(lastAddedOutdoorDate.addDays(1),
                                              QDate::currentDate(), false);
    oldDataUploader->addDataFromCurrentMonths(lastAddedIndoorDate.addDays(1),
                                              QDate::currentDate(), true);
    dataFromCurrentMonthsWasAdded = true;
}

void MainWindow::addDataFromLastDays() {
    extern const QString lastOutdoorTimestampRecordsCreationQuery;
    extern const QString lastIndoorTimestampRecordsCreationQuery;
    dbHandlerCopy->getResultFromDatabase(lastOutdoorTimestampRecordsCreationQuery);
    dbHandlerCopy->getResultFromDatabase(lastIndoorTimestampRecordsCreationQuery);
    oldDataUploader->addExtTimestampRecordsFromCurrentMonth();
    oldDataUploader->addIntTimestampRecordsFromCurrentMonth();

    dataFromLastDaysWasAdded = true;
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
    ephemerisPanel->setDate(dt.date());
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

void MainWindow::updateMinExtTemperatureTime(long long timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    const int positionToReplace = currentMinExtTempLabel->text().length() - 7;
    currentMinExtTempLabel->setText(currentMinExtTempLabel->text().replace(positionToReplace,
                                                           7,
                                                           dt.toString("(hh:mm)")));
}

void MainWindow::updateMaxExtTemperatureTime(long long timestamp) {
    QDateTime dt = QDateTime();
    dt.setSecsSinceEpoch(timestamp);
    const int positionToReplace = currentMaxExtTempLabel->text().length() - 7;
    currentMaxExtTempLabel->setText(currentMaxExtTempLabel->text().replace(positionToReplace,
                                                           7,
                                                           dt.toString("(hh:mm)")));
}

void MainWindow::updateExtExtremeTemperaturesInfo() {
    double minTemperature = analyzer->currentMinTemperatureInfo().first;
    long long minTemperatureTimestamp = analyzer->currentMinTemperatureInfo().second;
    double maxTemperature = analyzer->currentMaxTemperatureInfo().first;
    long long maxTemperatureTimestamp = analyzer->currentMaxTemperatureInfo().second;
    updateMinExtTemperature(minTemperature);
    updateMinExtTemperatureTime(minTemperatureTimestamp);
    updateMaxExtTemperature(maxTemperature);
    updateMaxExtTemperatureTime(maxTemperatureTimestamp);
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

void MainWindow::updatePredictionWidgets(WeatherPrediction prediction) {
    weatherPrediction = prediction;
    for (int i = 0; i < 4; i++) {
        predictionWidgets[i]->setMaximumTemperature(prediction.maxTemperature(i + 1));
        predictionWidgets[i]->setMinimumTemperature(prediction.minTemperature(i + 1));
        predictionWidgets[i]->setTitle(QDate::currentDate().addDays(i + 1).toString("d MMM"));
        predictionWidgets[i]->setMainPictogram(prediction.dayPictogram(i + 1));
    }
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
    q += "Date de fin : " + endDate + "\n";
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
    q += "Date de fin : " + endDate + "\n";
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
    indoorChart->setMeasurementType(_measurementType);
    indoorChart->setDurationInHours(_durationInHours);
    outdoorChart->setMeasurementType(_measurementType);
    outdoorChart->setDurationInHours(_durationInHours);
    emit recentDataShouldBeUpdated();
}

void MainWindow::showNormals() {
    NormalComputer *computer = new NormalComputer(dbHandlerCopy);
    NormalsVisualizer *visualizer = new NormalsVisualizer(computer);
    visualizer->show();
}

void MainWindow::showCredits() {
    QMessageBox::information(this, "Crédits",
                             "Auteur : Pierre OLLIVIER\n\n"
                             "API météo et icônes : Openweathermap");
}

void MainWindow::showPredictionWindow() {
    PredictionWindow *predictionWindow = new PredictionWindow(&weatherPrediction);
    predictionWindow->show();
}

void MainWindow::exploreData() {
    DataExplorator *explorator = new DataExplorator(dbHandlerCopy);
    explorator->show();
}

void MainWindow::postRecentDataRequests() {
    int dateBegin = QDateTime::currentDateTime().toSecsSinceEpoch() - _durationInHours * 3600 - 600;
    QString scale = "max";

    if (_durationInHours > 48) {
        scale = "30min";
    }
    if (accessToken != "") {
        recentDataHandler->postRequests(
                    dateBegin,
                    scale,
                    accessToken);
    }
}

QString MainWindow::measurementType() {
    return _measurementType;
}

int MainWindow::durationInHours() {
    return _durationInHours;
}

void MainWindow::setBackgroundColor(const QColor &color) {
    backgroundColor = color;
    // Ensure the widget is repainted with the new background color
    update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    // Paint the widget's background with the specified color
    QPainter painter(this);
    painter.fillRect(rect(), backgroundColor);
}
