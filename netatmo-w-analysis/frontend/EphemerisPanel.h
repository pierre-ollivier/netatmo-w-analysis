#ifndef EPHEMERISPANEL_H
#define EPHEMERISPANEL_H

#include <QDate>
#include <QDateTime>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include "../backend/DatabaseHandler.h"
#include "../backend/MetricsAnalyzer.h"
#include "../backend/NormalComputer.h"

class EphemerisPanel : public QWidget
{
    Q_OBJECT
public:
    EphemerisPanel();
    void setDate(QDate date);

public slots:
    void updateStdevLabel();
    void updateStatistics();
    void setSunrise(QDateTime sunriseDateTime);
    void setSunset(QDateTime sunsetDateTime);

private:
    QGroupBox *mainGroupBox;

    QLabel *txxLabel;
    QLabel *ephemerisLabel;
    QLabel *tnnLabel;
    QLabel *txmLabel;
    QLabel *tnmLabel;
    QLabel *stdevLabel;

    QLocale *deviceLocale;

    QGridLayout *boxLayout;
    QGridLayout *mainLayout;

    QDateTime *_sunriseDateTime;
    QDateTime *_sunsetDateTime;

    DatabaseHandler *dbHandler;
    NormalComputer *normalComputer;
    MetricsAnalyzer *analyzer;

    QDate _date;
};

#endif // EPHEMERISPANEL_H
