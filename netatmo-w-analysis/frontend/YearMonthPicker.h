#ifndef YEARMONTHPICKER_H
#define YEARMONTHPICKER_H

#include <QObject>
#include <QWidget>
#include <QDate>
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
    void setYear(int year);
    void setMonth(int month);
    void setDate(QDate date);
    void disableIrrelevantItems();

signals:
    void monthChanged(int);
    void yearChanged(int);

private:
    const int START_YEAR = 2019;
    const int CURRENT_YEAR = QDate::currentDate().year();
    const int CURRENT_MONTH = QDate::currentDate().month();
    const int NUMBER_OF_YEARS = CURRENT_YEAR - START_YEAR + 1;

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

    int _baseYear;
    int _baseMonth;

};

#endif // YEARMONTHPICKER_H
