#include "YearMonthPicker.h"
#include "../netatmo-w-analysis/frontend/CustomItemDelegate.h"
#include <QHeaderView>
#include <QDebug>
#include <QDate>

extern int START_YEAR;
extern QDate START_DATE;

YearMonthPicker::YearMonthPicker(int baseYear, int baseMonth, QWidget *parent) : QDialog(parent)
{
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
    disableIrrelevantItems();
}

void YearMonthPicker::handleMonthItemChanged(const QItemSelection &selection, const QItemSelection &_) {
    int row = selection.indexes()[0].row();
    int column = selection.indexes()[0].column();
    emit monthChanged(3 * row + column + 1);
    _baseMonth = (3 * row + column + 1);
    disableIrrelevantItems();
}

void YearMonthPicker::handleYearItemChanged(const QItemSelection &selection, const QItemSelection &_) {
    int row = selection.indexes()[0].row();
    emit yearChanged(row + START_YEAR);
    _baseYear = row + START_YEAR;
    disableIrrelevantItems();
}

void YearMonthPicker::setYear(int year) {
    _baseYear = year;
    yearView->selectionModel()->disconnect();
    yearView->selectionModel()->clearSelection();
    disableIrrelevantItems();
    yearView->selectionModel()->select(yearView->model()->index(_baseYear - START_YEAR, 0),
                                       QItemSelectionModel::Select);
    connect(
        yearView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        SLOT(handleYearItemChanged(const QItemSelection &, const QItemSelection &))
    );
}

void YearMonthPicker::setMonth(int month) {
    _baseMonth = month;
    monthView->selectionModel()->disconnect();
    monthView->selectionModel()->clearSelection();
    disableIrrelevantItems();
    monthView->selectionModel()->select(monthView->model()->index((_baseMonth - 1) / 3, (_baseMonth - 1) % 3),
                                        QItemSelectionModel::Select);
    connect(
        monthView->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        SLOT(handleMonthItemChanged(const QItemSelection &, const QItemSelection &))
    );
}

void YearMonthPicker::setDate(QDate date) {
    setYear(date.year());
    setMonth(date.month());
}

void YearMonthPicker::disableIrrelevantItems() {
    // provisionally enable all items
    for (int month = 0; month <= 11; month++) monthModel->item(month / 3, month % 3)->setEnabled(true);
    for (int year = 0; year < NUMBER_OF_YEARS; year++) yearModel->item(year)->setEnabled(true);

    // when the month is < START_DATE.month(), disable year START_YEAR
    if (_baseMonth < START_DATE.month()) yearModel->item(0)->setEnabled(false);

    // when the month is > CURRENT_MONTH, disable year CURRENT_YEAR
    if (_baseMonth > CURRENT_MONTH) yearModel->item(NUMBER_OF_YEARS - 1)->setEnabled(false);

    // when the year is START_YEAR, disable months < START_DATE.month()
    if (_baseYear == START_YEAR) {
        for (int month = 0; month < START_DATE.month() - 1; month++) {
            monthModel->item(month / 3, month % 3)->setEnabled(false);
        }
    }

    // when the year is CURRENT_YEAR, disable months > CURRENT_MONTH
    if (_baseYear == CURRENT_YEAR) {for (int month = CURRENT_MONTH; month <= 11; month++) monthModel->item(month / 3, month % 3)->setEnabled(false);}
}
