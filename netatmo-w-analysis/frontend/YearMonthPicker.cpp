#include "YearMonthPicker.h"
#include "../netatmo-w-analysis/frontend/CustomItemDelegate.h"
#include <QHeaderView>
#include <QDebug>

YearMonthPicker::YearMonthPicker(int baseYear, int baseMonth, QWidget *parent) : QDialog(parent)
{
    monthModel = new QStandardItemModel();
    yearModel = new QStandardItemModel();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            monthModel->setItem(j, i, new QStandardItem(abbreviatedMonths[3 * j + i]));
            monthModel->item(j, i)->setEditable(false);
            monthModel->item(j, i)->setTextAlignment(Qt::AlignCenter);
        }
    }

    for (int i = 0; i < 5; i++) {
        yearModel->setItem(i, 0, new QStandardItem(QString::number(2019 + i)));
        yearModel->item(i, 0)->setEditable(false);
        yearModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);
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
    monthView->setItemDelegate(new CustomItemDelegate());

    yearView->horizontalHeader()->hide();
    yearView->verticalHeader()->hide();
    yearView->setSelectionMode(QAbstractItemView::SingleSelection);
    yearView->selectionModel()->select(yearView->model()->index(baseYear - 2019, 0),
                                       QItemSelectionModel::Select);
    yearView->setItemDelegate(new CustomItemDelegate());

    for (int column = 0; column < 3; column++) {
        monthView->setColumnWidth(column, 40);
    }
    for (int row = 0; row < 4; row++) {
        monthView->setRowHeight(row, 50);
    }

    monthView->setFixedSize(149, 202);

    yearView->setColumnWidth(0, 50);
    yearView->setFixedSize(50, 250);

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(yearView);
    mainLayout->addWidget(monthView);

    setLayout(mainLayout);

    connect(
        monthView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        SLOT(handleMonthItemChanged(const QItemSelection &, const QItemSelection &))
    );

    connect(
        yearView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        SLOT(handleYearItemChanged(const QItemSelection &, const QItemSelection &))
    );

    setModal(true);
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
