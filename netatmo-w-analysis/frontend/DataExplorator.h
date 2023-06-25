#ifndef DATAEXPLORATOR_H
#define DATAEXPLORATOR_H

#include <QWidget>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTableView>

class DataExplorator : public QWidget
{
public:
    DataExplorator();

private:
    QGridLayout *layout;

    QTableView *mainView;
    QStandardItemModel *mainModel;
};

#endif // DATAEXPLORATOR_H
