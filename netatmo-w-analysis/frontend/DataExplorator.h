#ifndef DATAEXPLORATOR_H
#define DATAEXPLORATOR_H

#include <QWidget>
#include <QComboBox>
#include <QLocale>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTableView>
#include "../backend/DatabaseHandler.h"

class DataExplorator : public QWidget
{
    Q_OBJECT

public:
    DataExplorator(DatabaseHandler *dbHandler);

public slots:
    void fillBoard();

    std::vector<QVariant> getValues(
            QString operation,
            QString measurementCapitalized,
            QString monthCondition,
            QString order = "ASC");
    std::vector<QVariant> getValuesDates(
            QString operation,
            QString measurementCapitalized,
            QString monthCondition,
            QString order = "DESC");

private:
    QGridLayout *layout;

    QLocale *deviceLocale;

    QTableView *mainViewMax;
    QTableView *mainViewMin;
    QStandardItemModel *mainModelMax;
    QStandardItemModel *mainModelMin;

    QComboBox *monthComboBox;

    DatabaseHandler *_dbHandler;
};

#endif // DATAEXPLORATOR_H
