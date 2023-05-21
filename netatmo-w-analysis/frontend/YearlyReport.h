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
    QVariant getMaxTemperatureByDate(int day, int month);

signals:

private:
    QTableView *mainView;
    QStandardItemModel *mainModel;

    QHBoxLayout *mainLayout;
    QVBoxLayout *optionsLayout;

    DatabaseHandler *dbHandler;
};

#endif // YEARLYREPORT_H
