#include "YearlyReport.h"

YearlyReport::YearlyReport()
{
    mainView = new QTableView();
    mainModel = new QStandardItemModel();

    mainView->setModel(mainModel);

    mainLayout = new QHBoxLayout();
    optionsLayout = new QVBoxLayout();

    mainLayout->addWidget(mainView);
    mainLayout->addLayout(optionsLayout);

    setLayout(mainLayout);
}
