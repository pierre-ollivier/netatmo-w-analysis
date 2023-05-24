#ifndef YEARMONTHPICKER_H
#define YEARMONTHPICKER_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>

class YearMonthPicker : public QDialog
{
    Q_OBJECT
public:
    explicit YearMonthPicker(int baseYear, int baseMonth, QWidget *parent = nullptr);

public slots:
    void handleMonthItemChanged(const QItemSelection &, const QItemSelection &);
    void handleYearItemChanged(const QItemSelection &, const QItemSelection &);

signals:
    void monthChanged(int);
    void yearChanged(int);

private:
    const QStringList abbreviatedMonths = {
        "janv.",
        "févr.",
        "mars",
        "avr.",
        "mai",
        "juin",
        "juill.",
        "août",
        "sept.",
        "oct.",
        "nov.",
        "déc."
    };

    QTableView *monthView;
    QTableView *yearView;
    QStandardItemModel *monthModel;
    QStandardItemModel *yearModel;
    QHBoxLayout *mainLayout;

};

#endif // YEARMONTHPICKER_H
