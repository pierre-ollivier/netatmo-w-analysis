#include "ConditionWidget.h"

ConditionWidget::ConditionWidget(QWidget *parent) : QGroupBox(parent)
{
    conditionTypeComboBox = new QComboBox();
    conditionTypeComboBox->addItems(measurementsItems);
    conditionTypeComboBox->addItems(datesItems);

    operationComboBox = new QComboBox();
    operationComboBox->addItems(operationsItems);

    layout = new QGridLayout();
    layout->addWidget(conditionTypeComboBox, 1, 1);
    layout->addWidget(operationComboBox, 1, 2);
    setLayout(layout);
}
