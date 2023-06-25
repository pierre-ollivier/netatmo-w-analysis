#ifndef DATAEXPLORATOR_H
#define DATAEXPLORATOR_H

#include <QWidget>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTableView>
#include "../backend/DatabaseHandler.h"

class DataExplorator : public QWidget
{
public:
    DataExplorator(DatabaseHandler *dbHandler);

    void fillBoard();

private:
    QGridLayout *layout;

    QTableView *mainView;
    QStandardItemModel *mainModel;

    DatabaseHandler *_dbHandler;
};

#endif // DATAEXPLORATOR_H
