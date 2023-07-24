#ifndef EPHEMERISPANEL_H
#define EPHEMERISPANEL_H

#include <QDate>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include "../backend/DatabaseHandler.h"
#include "../backend/MetricsAnalyzer.h"
#include "../backend/NormalComputer.h"

class EphemerisPanel : public QGroupBox
{
    Q_OBJECT
public:
    EphemerisPanel();
    void setDate(QDate date);

public slots:
    void updateStdevLabel();

private:
    QLabel *txxLabel;
    QLabel *tnnLabel;
    QLabel *txmLabel;
    QLabel *tnmLabel;
    QLabel *stdevLabel;
    QLocale *deviceLocale;
    QGridLayout *layout;
    DatabaseHandler *dbHandler;
    NormalComputer *normalComputer;
    MetricsAnalyzer *analyzer;
};

#endif // EPHEMERISPANEL_H
