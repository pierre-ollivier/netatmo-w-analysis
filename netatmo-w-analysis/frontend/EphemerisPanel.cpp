#include "EphemerisPanel.h"

extern const QString PATH_TO_PROD_DATABASE;

EphemerisPanel::EphemerisPanel() : QGroupBox()
{
    txxLabel = new QLabel("__,_ °C");

    deviceLocale = new QLocale();

    layout = new QGridLayout();

    layout->addWidget(new QLabel("Température maximale : "), 1, 1);
    layout->addWidget(txxLabel, 1, 2);

    dbHandler = new DatabaseHandler(PATH_TO_PROD_DATABASE);

    setTitle("Statistiques pour un _ _");
    setLayout(layout);
    setAlignment(Qt::AlignHCenter);
}

void EphemerisPanel::setDate(QDate date) {
    setTitle("Statistiques pour un " + date.toString("dd MMMM"));
    double txx = dbHandler->getResultFromDatabase(
                "SELECT max(maxTemperature) FROM OutdoorDailyRecords "
                "WHERE month = " + QString::number(date.month()) + " "
                "AND day = " + QString::number(date.day())).toDouble();
    txxLabel->setText(deviceLocale->toString(txx, 'f', 1) + " °C");
}
