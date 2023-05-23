#ifndef YEARLYREPORT_H
#define YEARLYREPORT_H

#include <QWidget>
#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class YearlyReport : public QWidget
{
    Q_OBJECT
public:
    YearlyReport();

public slots:
    void fillBoard();
    QVariant getMaxMaxMeasurementByDate(QString measurement, int day, int month);
    int getMaxMaxMeasurementYearByDate(QString measurement, int day, int month, double maxMaxMeasurement);
    QVariant getMinMinTemperatureByDate(int day, int month);
    int getMinMinTemperatureYearByDate(int day, int month, double minMinTemperature);
    QVariant getMaxMinTemperatureByDate(int day, int month);
    int getMaxMinTemperatureYearByDate(int day, int month, double maxMinTemperature);
    QVariant getMinMaxTemperatureByDate(int day, int month);
    int getMinMaxTemperatureYearByDate(int day, int month, double minMaxTemperature);
    QVariant getAvgMaxTemperatureByDate(int day, int month);
    QVariant getAvgMinTemperatureByDate(int day, int month);

signals:

private:
    QString capitalize(QString s);
    QTableView *mainView;
    QStandardItemModel *mainModel;

    QLocale *deviceLocale;

    QHBoxLayout *mainLayout;
    QVBoxLayout *optionsLayout;

    QRadioButton *temperatureRadioButton;
    QRadioButton *humidityRadioButton;
    QRadioButton *dewPointRadioButton;
    QRadioButton *humidexRadioButton;
    QCheckBox *interiorCheckBox;

    DatabaseHandler *dbHandler;
};

#endif // YEARLYREPORT_H
