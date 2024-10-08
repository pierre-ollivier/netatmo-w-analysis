#ifndef MONTHLYREPORT_H
#define MONTHLYREPORT_H

#include <QWidget>
#include <QDate>
#include <QTableView>
#include <QStandardItemModel>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include "../netatmo-w-analysis/frontend/YearMonthPicker.h"
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class MonthlyReport : public QWidget
{
    Q_OBJECT

public:
    MonthlyReport();

public slots:
    void destroy();
    void fillBoard();
    void add1Month();
    void substract1Month();
    void setMonth(int month);
    void setYear(int year);
    void changeMeasurement();

private:
    QDate *initialDate;
    QTableView *view;
    QStandardItemModel *model;
    QGridLayout *layout;
    QPushButton *currentMonthClickableLabel;
    QPushButton *add1MonthButton;
    QPushButton *substract1MonthButton;
    YearMonthPicker *yearMonthPicker;

    QRadioButton *temperatureRadioButton;
    QRadioButton *humidityRadioButton;
    QRadioButton *dewPointRadioButton;
    QRadioButton *humidexRadioButton;
    QRadioButton *pressureRadioButton;
    QCheckBox *interiorCheckBox;
    QVBoxLayout *buttonsLayout;

    QDate *_date;
    DatabaseHandler *dbHandlerCopy;

    QString measurementType = "temperature";
    QString abbreviatedMeasurement = "T.";
    QString unit = "°C";
    QString indoorOrOutdoorCapitalized = "Outdoor";
    QString extraWhereClause = "";
    int decimals = 1;
};

#endif // MONTHLYREPORT_H
