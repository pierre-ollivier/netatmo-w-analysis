#include "EphemerisPanel.h"
#include <QDebug>

extern const QString PATH_TO_COPY_DATABASE;

EphemerisPanel::EphemerisPanel() : QGroupBox()
{
    txxLabel = new QLabel("__,_ °C (____)");
    tnnLabel = new QLabel("__,_ °C (____)");
    txmLabel = new QLabel("__,_ °C");
    tnmLabel = new QLabel("__,_ °C");
    stdevLabel = new QLabel("_________________________");

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
    layout->addWidget(new QLabel("Écarts aux normales :"), 5, 1);
    layout->addWidget(stdevLabel, 5, 2);

    dbHandler = new DatabaseHandler(PATH_TO_COPY_DATABASE);
    normalComputer = new NormalComputer(dbHandler);
    analyzer = new MetricsAnalyzer();

    setTitle("Statistiques pour un _ _");
    setFont(QFont("Arial", 12));
    setLayout(layout);
    setAlignment(Qt::AlignHCenter);
    setStyleSheet(
                "QGroupBox {"
                "  border: 2px solid gray;"
                "  border-radius: 15px;"
                "  margin-top: 10px;"
                "}"
                "QGroupBox::title {"
                "  subcontrol-origin: margin;"
                "  subcontrol-position: top center;"
                "}");
}


void EphemerisPanel::setDate(QDate date) {
    if (date != _date) {
        _date = date;
        updateStatistics();
    }
}


void EphemerisPanel::updateStatistics() {
    setTitle("Statistiques pour un " + _date.toString("d MMMM"));
    if (_date.day() == 1) setTitle("Statistiques pour un " + _date.toString("der MMMM"));

    double txx = dbHandler->getResultFromDatabase(
                "SELECT max(maxTemperature) FROM OutdoorDailyRecords "
                "WHERE month = " + QString::number(_date.month()) + " "
                "AND day = " + QString::number(_date.day())).toDouble();
    txxLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(txx, 'f', 1) + "</font> °C");

    double tnn = dbHandler->getResultFromDatabase(
                "SELECT min(minTemperature) FROM OutdoorDailyRecords "
                "WHERE month = " + QString::number(_date.month()) + " "
                "AND day = " + QString::number(_date.day())).toDouble();
    tnnLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(tnn, 'f', 1) + "</font> °C");

    double txm = normalComputer->normalMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                "maxTemperature",
                41);
    txmLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(txm, 'f', 1) + "</font> °C");

    double tnm = normalComputer->normalMeasurementByMovingAverage(
                "OutdoorDailyRecords",
                _date,
                "minTemperature",
                41);
    tnmLabel->setText("<font color = \"#209fdf\">" + deviceLocale->toString(tnm, 'f', 1) + "</font> °C");

    int txxYear = dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(_date.day()) + " "
                "AND month = " + QString::number(_date.month()) + " "
                "AND maxTemperature = " + QString::number(txx)).toInt();
    txxLabel->setText(txxLabel->text() + " (" + QString::number(txxYear) + ")");

    int tnnYear = dbHandler->getResultFromDatabase(
                "SELECT year FROM OutdoorDailyRecords "
                "WHERE day = " + QString::number(_date.day()) + " "
                "AND month = " + QString::number(_date.month()) + " "
                "AND minTemperature = " + QString::number(tnn)).toInt();
    tnnLabel->setText(tnnLabel->text() + " (" + QString::number(tnnYear) + ")");
    updateStdevLabel();
}


void EphemerisPanel::updateStdevLabel() {
    stdevLabel->setText(analyzer->text(dbHandler));
}
