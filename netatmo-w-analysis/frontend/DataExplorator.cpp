#include "DataExplorator.h"

DataExplorator::DataExplorator() : QWidget()
{
    mainModel = new QStandardItemModel(5, 2);
    mainModel->setHorizontalHeaderLabels(
                QStringList({"Température maximale", "Année"}));

    mainView = new QTableView();
    mainView->setModel(mainModel);

    layout = new QGridLayout();
    layout->addWidget(mainView, 1, 1);
    setLayout(layout);
}
