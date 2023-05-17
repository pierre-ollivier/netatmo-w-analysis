#ifndef YEARMONTHPICKER_H
#define YEARMONTHPICKER_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>

class YearMonthPicker : public QWidget
{
    Q_OBJECT
public:
    explicit YearMonthPicker(QWidget *parent = nullptr);

signals:

private:
    const QStringList abbreviatedMonths = {
        "jan.",
        "fév.",
        "mar.",
        "avr.",
        "mai",
        "jun.",
        "jul.",
        "aoû.",
        "sep.",
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
