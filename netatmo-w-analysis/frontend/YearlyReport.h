#ifndef YEARLYREPORT_H
#define YEARLYREPORT_H

#include <QWidget>
#include <QObject>
#include <QString>
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
    QVariant getMinMinMeasurementByDate(QString measurement, int day, int month);
    int getMinMinMeasurementYearByDate(QString measurement, int day, int month, double minMinMeasurement);
    QVariant getMaxMinMeasurementByDate(QString measurement, int day, int month);
    int getMaxMinMeasurementYearByDate(QString measurement, int day, int month, double maxMinMeasurement);
    QVariant getMinMaxMeasurementByDate(QString measurement, int day, int month);
    int getMinMaxMeasurementYearByDate(QString measurement, int day, int month, double minMaxMeasurement);
    QVariant getAvgMaxMeasurementByDate(QString measurement, int day, int month);
    QVariant getAvgMinMeasurementByDate(QString measurement, int day, int month);

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
