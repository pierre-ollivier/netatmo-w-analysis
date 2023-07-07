#include "ConditionWidget.h"

ConditionWidget::ConditionWidget(QWidget *parent) : QGroupBox(parent)
{
    measurementComboBox = new QComboBox();
    measurementComboBox->addItems(measurementsItems);
    dateComboBox = new QComboBox();
    dateComboBox->addItems(datesItems);

    layout = new QGridLayout();
    layout->addWidget(measurementComboBox);
    layout->addWidget(dateComboBox);
    setLayout(layout);
}
