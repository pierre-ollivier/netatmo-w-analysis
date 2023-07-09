#include "ConditionWidget.h"

ConditionWidget::ConditionWidget(QWidget *parent) : QGroupBox(parent)
{
    conditionTypeComboBox = new QComboBox();
    conditionTypeComboBox->addItems(measurementsItems);
    conditionTypeComboBox->addItems(datesItems);

    operationComboBox = new QComboBox();
    operationComboBox->addItems(operationsItems);

    firstValueInput = new QLineEdit();
    firstValueInput->setFixedWidth(50);

    layout = new QGridLayout();
    layout->addWidget(conditionTypeComboBox, 1, 1);
    layout->addWidget(operationComboBox, 1, 2);
    layout->addWidget(firstValueInput, 1, 3);
    setLayout(layout);
}
