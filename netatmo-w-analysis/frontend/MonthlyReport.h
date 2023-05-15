#ifndef MONTHLYREPORT_H
#define MONTHLYREPORT_H

#include <QWidget>
#include <QDate>
#include <QTableView>
#include <QStandardItemModel>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include "../netatmo-w-analysis/backend/DatabaseHandler.h"

class MonthlyReport : public QWidget
{
public:
    MonthlyReport();

public slots:
    void destroy();
    void fillBoard();
    void add1Month();
    void substract1Month();
    void majType();

protected:

    QColor temperatureColor(double temperature);
    QColor humidityColor(int humidity);

private:

    QDate *initialDate;
    QTableView *view;
    QStandardItemModel *model;
    QGridLayout *layout;
    QLabel *currentMonthLabel;
    QPushButton *add1MonthButton;
    QPushButton *substract1MonthButton;

//    QRadioButton *rbT;
//    QRadioButton *rbRh;
//    QRadioButton *rbTd;
//    QRadioButton *rbHx;
//    QCheckBox *boxInt;
    QVBoxLayout *buttonsLayout;

    QDate *_date;
    DatabaseHandler *dbHandler;
    QLocale *deviceLocale;
};

#endif // MONTHLYREPORT_H
