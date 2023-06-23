#ifndef EPHEMERISPANEL_H
#define EPHEMERISPANEL_H

#include <QDate>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include "../backend/DatabaseHandler.h"
#include "../backend/NormalComputer.h"

class EphemerisPanel : public QGroupBox
{
public:
    EphemerisPanel();

    void setDate(QDate date);

private:
    QLabel *txxLabel;
    QLabel *tnnLabel;
    QLabel *txmLabel;
    QLabel *tnmLabel;
    QLocale *deviceLocale;
    QGridLayout *layout;
    DatabaseHandler *dbHandler;
    NormalComputer *normalComputer;
};

#endif // EPHEMERISPANEL_H
