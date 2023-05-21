#include "YearlyReport.h"
#include <QDate>

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

    fillBoard();
}

void YearlyReport::fillBoard() {
    for (QDate date = QDate(2000, 1, 1); date <= QDate(2000, 12, 31); date = date.addDays(1)) {
        int row = date.dayOfYear();
        mainModel->setItem(row, 0, new QStandardItem());
        mainModel->setVerticalHeaderItem(row, new QStandardItem(date.toString("dd/MM")));
    }
}
