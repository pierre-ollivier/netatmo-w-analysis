#ifndef YEARMONTHPICKER_H
#define YEARMONTHPICKER_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
    QPushButton *monthButtons[12];
    QPushButton *yearButtons[5];
    QGridLayout *monthButtonsLayout;
    QVBoxLayout *yearButtonsLayout;
    QHBoxLayout *mainLayout;

};

#endif // YEARMONTHPICKER_H
