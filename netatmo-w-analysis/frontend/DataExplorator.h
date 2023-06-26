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

private:
    QGridLayout *layout;

    QLocale *deviceLocale;

    QTableView *mainView;
    QStandardItemModel *mainModel;

    QComboBox *monthComboBox;

    DatabaseHandler *_dbHandler;
};

#endif // DATAEXPLORATOR_H
