#include "EphemerisPanel.h"
#include <QDebug>

extern const QLocale LOCALE;
extern const QString PATH_TO_COPY_DATABASE;

EphemerisPanel::EphemerisPanel() : QWidget()
{
    mainGroupBox = new QGroupBox();

    _sunriseDateTime = new QDateTime();
    _sunsetDateTime = new QDateTime();

    ephemerisLabel = new QLabel("Lever du soleil : __:__ - Coucher du soleil : __:__");
    ephemerisLabel->setFont(QFont("Arial", 9));
    ephemerisLabel->setAlignment(Qt::AlignCenter);

    txxLabel = new QLabel("__,_ °C (____)");
    tnnLabel = new QLabel("__,_ °C (____)");
    txmLabel = new QLabel("__,_ °C");
    tnmLabel = new QLabel("__,_ °C");
    stdevLabel = new QLabel("_________________________");

    deviceLocale = new QLocale(LOCALE);

    mainLayout = new QGridLayout();
    boxLayout = new QGridLayout();

    boxLayout->addWidget(ephemerisLabel, 0, 1, 1, 2);
    boxLayout->addWidget(new QLabel("Température maximale absolue : "), 1, 1);
    boxLayout->addWidget(txxLabel, 1, 2);
    boxLayout->addWidget(new QLabel("Température minimale absolue : "), 2, 1);
    boxLayout->addWidget(tnnLabel, 2, 2);
    boxLayout->addWidget(new QLabel("Température maximale moyenne : "), 3, 1);
    boxLayout->addWidget(txmLabel, 3, 2);
    boxLayout->addWidget(new QLabel("Température minimale moyenne : "), 4, 1);
    boxLayout->addWidget(tnmLabel, 4, 2);
    boxLayout->addWidget(stdevLabel, 5, 1, 1, 2);

    dbHandler = new DatabaseHandler(this, PATH_TO_COPY_DATABASE);
    normalComputer = new NormalComputer(this, dbHandler);
    analyzer = new MetricsAnalyzer(this);

    mainGroupBox->setTitle("Statistiques pour un _ _");
    mainGroupBox->setFont(QFont("Arial", 12));
    mainGroupBox->setLayout(boxLayout);
    mainGroupBox->setAlignment(Qt::AlignHCenter);
    mainGroupBox->setStyleSheet(
                "QGroupBox {"
                "  border: 2px solid gray;"
                "  border-radius: 15px;"
                "  margin-top: 10px;"
                "}"
                "QGroupBox::title {"
                "  subcontrol-origin: margin;"
                "  subcontrol-position: top center;"
                "}");

    mainLayout->addWidget(mainGroupBox, 1, 1);
    setLayout(mainLayout);
}


void EphemerisPanel::setDate(QDate date) {
    if (date != _date) {
        _date = date;
        updateStatistics();
    }
}


void EphemerisPanel::updateStatistics() {
    mainGroupBox->setTitle("Statistiques pour un " + deviceLocale->toString(_date, "d MMMM"));
    if (_date.day() == 1) mainGroupBox->setTitle("Statistiques pour un " + deviceLocale->toString(_date, "der MMMM"));

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

void EphemerisPanel::setSunrise(QDateTime sunriseDateTime) {
    *_sunriseDateTime = sunriseDateTime;
    ephemerisLabel->setText("Lever du soleil : " + _sunriseDateTime->toString("hh:mm")
                            + " - Coucher du soleil : " + _sunsetDateTime->toString("hh:mm"));
}

void EphemerisPanel::setSunset(QDateTime sunsetDateTime) {
    *_sunsetDateTime = sunsetDateTime;
    ephemerisLabel->setText("Lever du soleil : " + _sunriseDateTime->toString("hh:mm")
                            + " - Coucher du soleil : " + _sunsetDateTime->toString("hh:mm"));
}
