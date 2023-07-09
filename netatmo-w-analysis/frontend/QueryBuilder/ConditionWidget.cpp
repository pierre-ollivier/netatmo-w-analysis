#include "ConditionWidget.h"

ConditionWidget::ConditionWidget(QWidget *parent) : QGroupBox(parent)
{
    conditionTypeComboBox = new QComboBox();
    conditionTypeComboBox->addItems(measurementsItems);
    conditionTypeComboBox->addItems(datesItems);
    connect(conditionTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            SLOT(changeMinMaxComboBoxVisibility()));

    minMaxComboBox = new QComboBox();
    minMaxComboBox->addItems({"max.", "min.", "moy.", "var."});
    minMaxComboBox->setFixedWidth(55);
    minMaxComboBox->setVisible(false);

    operationComboBox = new QComboBox();
    operationComboBox->addItems(operationsItems);
    operationComboBox->setFixedWidth(155);

    firstValueInput = new QLineEdit();
    firstValueInput->setFixedWidth(50);

    layout = new QGridLayout();
    layout->addWidget(conditionTypeComboBox, 1, 1);
    layout->addWidget(minMaxComboBox, 1, 2);
    layout->addWidget(operationComboBox, 1, 3);
    layout->addWidget(firstValueInput, 1, 4);
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

void ConditionWidget::changeMinMaxComboBoxVisibility() {
    minMaxComboBox->setVisible(1 <= conditionTypeComboBox->currentIndex()
                               && conditionTypeComboBox->currentIndex() <= 4);
}
