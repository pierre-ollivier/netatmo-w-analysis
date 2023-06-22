#include "EphemerisPanel.h"

extern const QString PATH_TO_PROD_DATABASE;

EphemerisPanel::EphemerisPanel() : QGroupBox()
{
    txxLabel = new QLabel("__,_ °C");
    tnnLabel = new QLabel("__,_ °C");
    txmLabel = new QLabel("__,_ °C");
    tnmLabel = new QLabel("__,_ °C");

    deviceLocale = new QLocale();

    layout = new QGridLayout();

    layout->addWidget(new QLabel("Température maximale absolue : "), 1, 1);
    layout->addWidget(txxLabel, 1, 2);
    layout->addWidget(new QLabel("Température minimale absolue : "), 2, 1);
    layout->addWidget(tnnLabel, 2, 2);
    layout->addWidget(new QLabel("Température maximale moyenne : "), 3, 1);
    layout->addWidget(txmLabel, 3, 2);
    layout->addWidget(new QLabel("Température minimale moyenne : "), 4, 1);
    layout->addWidget(tnmLabel, 4, 2);

    dbHandler = new DatabaseHandler(PATH_TO_PROD_DATABASE);
    normalComputer = new NormalComputer(dbHandler);

    setTitle("Statistiques pour un _ _");
    setFont(QFont("Arial", 12));
    setLayout(layout);
    setAlignment(Qt::AlignHCenter);
}

void EphemerisPanel::setDate(QDate date) {
    setTitle("Statistiques pour un " + date.toString("dd MMMM"));

    double txx = dbHandler->getResultFromDatabase(
                "SELECT max(maxTemperature) FROM OutdoorDailyRecords "
                "WHERE month = " + QString::number(date.month()) + " "
                "AND day = " + QString::number(date.day())).toDouble();
    txxLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(txx, 'f', 1) + "</font> °C");

    double tnn = dbHandler->getResultFromDatabase(
                "SELECT min(minTemperature) FROM OutdoorDailyRecords "
                "WHERE month = " + QString::number(date.month()) + " "
                "AND day = " + QString::number(date.day())).toDouble();
    tnnLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(tnn, 'f', 1) + "</font> °C");

    double txm = normalComputer->normalMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                date,
                "maxTemperature",
                41);
    txmLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(txm, 'f', 1) + "</font> °C");

    double tnm = normalComputer->normalMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                date,
                "minTemperature",
                41);
    tnmLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(tnm, 'f', 1) + "</font> °C");

    int txxYear = dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(date.day()) + " "
                "AND month = " + QString::number(date.month()) + " "
                "AND maxTemperature = " + QString::number(txx)).toInt();
    txxLabel->setText(txxLabel->text() + " (" +QString::number(txxYear) + ")");

    int tnnYear = dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(date.day()) + " "
                "AND month = " + QString::number(date.month()) + " "
                "AND minTemperature = " + QString::number(tnn)).toInt();
    tnnLabel->setText(tnnLabel->text() + " (" +QString::number(tnnYear) + ")");
}
