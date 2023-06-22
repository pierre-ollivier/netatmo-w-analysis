#ifndef EPHEMERISPANEL_H
#define EPHEMERISPANEL_H

#include <QDate>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include "../backend/DatabaseHandler.h"

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
};

#endif // EPHEMERISPANEL_H
