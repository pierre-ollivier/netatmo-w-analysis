#ifndef YEARLYREPORT_H
#define YEARLYREPORT_H

#include <QWidget>
#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class YearlyReport : public QWidget
{
    Q_OBJECT
public:
    YearlyReport();

public slots:
    void fillBoard();
    QVariant getMaxMaxTemperatureByDate(int day, int month);
    int getMaxMaxTemperatureYearByDate(int day, int month, double maxMaxTemperature);
    QVariant getMinMinTemperatureByDate(int day, int month);
    int getMinMinTemperatureYearByDate(int day, int month, double minMinTemperature);
    QVariant getMaxMinTemperatureByDate(int day, int month);
    int getMaxMinTemperatureYearByDate(int day, int month, double maxMinTemperature);
    QVariant getMinMaxTemperatureByDate(int day, int month);
    int getMinMaxTemperatureYearByDate(int day, int month, double minMaxTemperature);

signals:

private:
    QTableView *mainView;
    QStandardItemModel *mainModel;

    QLocale *deviceLocale;

    QHBoxLayout *mainLayout;
    QVBoxLayout *optionsLayout;

    DatabaseHandler *dbHandler;
};

#endif // YEARLYREPORT_H
