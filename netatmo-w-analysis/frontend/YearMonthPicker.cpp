#include "YearMonthPicker.h"
#include "../netatmo-w-analysis/frontend/CustomItemDelegate.h"
#include <QHeaderView>
#include <QDebug>
#include <QDate>

YearMonthPicker::YearMonthPicker(int baseYear, int baseMonth, QWidget *parent) : QDialog(parent)
{
    const int START_YEAR = 2019;
    const int NUMBER_OF_YEARS = QDate::currentDate().year() - START_YEAR + 1;
    _baseYear = baseYear;
    _baseMonth = baseMonth;
    monthModel = new QStandardItemModel();
    yearModel = new QStandardItemModel();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            monthModel->setItem(j, i, new QStandardItem(abbreviatedMonths[3 * j + i]));
            monthModel->item(j, i)->setEditable(false);
            monthModel->item(j, i)->setTextAlignment(Qt::AlignCenter);
        }
    }

    for (int i = 0; i < NUMBER_OF_YEARS; i++) {
        yearModel->setItem(i, 0, new QStandardItem(QString::number(START_YEAR + i)));
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
    monthView->selectionModel()->select(monthView->model()->index((_baseMonth - 1) / 3, (_baseMonth - 1) % 3),
                                        QItemSelectionModel::Select);
    monthView->setItemDelegate(new CustomItemDelegate());

    yearView->horizontalHeader()->hide();
    yearView->verticalHeader()->hide();
    yearView->setSelectionMode(QAbstractItemView::SingleSelection);
    yearView->selectionModel()->select(yearView->model()->index(_baseYear - START_YEAR, 0),
                                       QItemSelectionModel::Select);
    yearView->setItemDelegate(new CustomItemDelegate());

    for (int column = 0; column < 3; column++) {
        monthView->setColumnWidth(column, 40);
    }
    for (int row = 0; row < 4; row++) {
        monthView->setRowHeight(row, 50);
    }

    for (int row = 0; row < NUMBER_OF_YEARS; row++) {
        yearView->setRowHeight(row, 240 / NUMBER_OF_YEARS);
    }

    monthView->setFixedSize(149, 202);

    yearView->setColumnWidth(0, 50);
    yearView->setFixedSize(50, 242);

    mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(60);
    mainLayout->addWidget(yearView);
    mainLayout->addWidget(monthView);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

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

void YearMonthPicker::setYear(int year) {
    _baseYear = year;
}

void YearMonthPicker::setMonth(int month) {
    _baseMonth = month;
}

void YearMonthPicker::setDate(QDate date) {
    _baseYear = date.year();
    _baseMonth = date.month();
}
