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

QString ConditionWidget::condition() {
    QString cond = translatedMeasurementsItems[conditionTypeComboBox->currentIndex()];

    switch (operationComboBox->currentIndex()) {
    case 0:
        cond += " = " + firstValueInput->text();
        break;
    case 1:
        cond += " = " + firstValueInput->text();
        break;
    case 2:
        cond += " > " + firstValueInput->text();
        break;
    case 3:
        cond += " >= " + firstValueInput->text();
        break;
    case 4:
        cond += " < " + firstValueInput->text();
        break;
    case 5:
        cond += " <= " + firstValueInput->text();
        break;
    default:
        cond += " TODO ";  // TODO
        break;
    }

    return cond;
}
