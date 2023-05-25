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
    QVariant getMeasurementByDate(QString measurement, QString operation, QString scope, int day, int month);
    int getMeasurementYearByDate(QString measurementType, QString scope, int day, int month, double measurementValue);
    void changeMeasurement();

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

    QString measurementType = "temperature";
    QStringList legends = QStringList({
                                          "Tnn",
                                          "Txn",
                                          "Tnx",
                                          "Txx",
                                          "Température minimale moyenne",
                                          "Température maximale moyenne"
                                      });
    QString unit = "°C";
    int decimals = 1;

    DatabaseHandler *dbHandler;
};

#endif // YEARLYREPORT_H
