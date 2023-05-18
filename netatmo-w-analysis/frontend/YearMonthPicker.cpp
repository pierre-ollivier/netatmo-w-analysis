#include "YearMonthPicker.h"
#include <QHeaderView>
#include <QDebug>

YearMonthPicker::YearMonthPicker(int baseYear, int baseMonth, QWidget *parent) : QWidget(parent)
{
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
    monthView->setSelectionMode(QAbstractItemView::SingleSelection);
    monthView->selectionModel()->select(monthView->model()->index((baseMonth - 1) / 3, (baseMonth - 1) % 3),
                                        QItemSelectionModel::Select);

    yearView->horizontalHeader()->hide();
    yearView->verticalHeader()->hide();
    yearView->setSelectionMode(QAbstractItemView::SingleSelection);
    yearView->selectionModel()->select(yearView->model()->index(baseYear - 2019, 0),
                                       QItemSelectionModel::Select);

    for (int column = 0; column < 3; column++) {
        monthView->setColumnWidth(column, 40);
    }

    yearView->setColumnWidth(0, 50);

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(yearView);
    mainLayout->addWidget(monthView);

    setLayout(mainLayout);

    connect(
        monthView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        SLOT(handleMonthItemChanged(const QItemSelection &, const QItemSelection &))
    );
}

void YearMonthPicker::handleMonthItemChanged(const QItemSelection &selection, const QItemSelection &_) {
    int row = selection.indexes()[0].row();
    int column = selection.indexes()[0].column();
    emit monthChanged(3 * row + column + 1);
}

void YearMonthPicker::handleYearItemChanged(const QItemSelection &selection, const QItemSelection &_) {
    int row = selection.indexes()[0].row();
    emit yearChanged(row + 2019);
}
