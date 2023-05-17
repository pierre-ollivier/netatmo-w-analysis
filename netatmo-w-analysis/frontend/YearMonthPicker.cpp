#include "YearMonthPicker.h"
#include <QHeaderView>

YearMonthPicker::YearMonthPicker(QWidget *parent) : QWidget(parent)
{
//    monthButtonsLayout = new QGridLayout();
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 4; j++) {
//            monthButtons[3 * j + i] = new QPushButton(abbreviatedMonths[3 * j + i]);
//            monthButtonsLayout->addWidget(monthButtons[3 * j + i], j, i);
//        }
//    }
//    yearButtonsLayout = new QVBoxLayout();
//    for (int i = 0; i < 5; i++) {
//        yearButtons[i] = new QPushButton(QString::number(2019 + i));
//        yearButtonsLayout->addWidget(yearButtons[i]);
//    }
//    mainLayout = new QHBoxLayout();
//    mainLayout->addLayout(yearButtonsLayout);
//    mainLayout->addLayout(monthButtonsLayout);

    monthModel = new QStandardItemModel();
    yearModel = new QStandardItemModel();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            monthModel->setItem(j, i, new QStandardItem(abbreviatedMonths[3 * j + i]));
        }
    }

    for (int i = 0; i < 5; i++) {
        yearModel->setItem(i, 0, new QStandardItem(QString::number(2019 + i)));
    }

    monthView = new QTableView();
    yearView = new QTableView();

    monthView->setModel(monthModel);
    yearView->setModel(yearModel);

    monthView->horizontalHeader()->hide();
    monthView->verticalHeader()->hide();

    yearView->horizontalHeader()->hide();
    yearView->verticalHeader()->hide();

    for (int column = 0; column < 3; column++) {
        monthView->setColumnWidth(column, 40);
    }

    yearView->setColumnWidth(0, 50);

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(yearView);
    mainLayout->addWidget(monthView);

    setLayout(mainLayout);
}
