#include "YearMonthPicker.h"

YearMonthPicker::YearMonthPicker(QWidget *parent) : QWidget(parent)
{
    monthButtonsLayout = new QGridLayout();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            monthButtons[3 * j + i] = new QPushButton(abbreviatedMonths[3 * j + i]);
            monthButtonsLayout->addWidget(monthButtons[3 * j + i], j, i);
        }
    }
    yearButtonsLayout = new QVBoxLayout();
    for (int i = 0; i < 5; i++) {
        yearButtons[i] = new QPushButton(QString::number(2019 + i));
        yearButtonsLayout->addWidget(yearButtons[i]);
    }
    mainLayout = new QHBoxLayout();
    mainLayout->addLayout(yearButtonsLayout);
    mainLayout->addLayout(monthButtonsLayout);
    setLayout(mainLayout);
}
