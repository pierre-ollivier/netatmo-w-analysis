#include "ConditionWidget.h"

ConditionWidget::ConditionWidget(QWidget *parent) : QGroupBox(parent)
{
    conditionTypeComboBox = new QComboBox();
    conditionTypeComboBox->addItems(measurementsItems);
    conditionTypeComboBox->addItems(datesItems);
    connect(conditionTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            SLOT(changeMinMaxComboBoxVisibility()));
    connect(conditionTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            SIGNAL(conditionChanged()));

    minMaxComboBox = new QComboBox();
    minMaxComboBox->addItems(minMaxItems);
    minMaxComboBox->setFixedWidth(55);
    minMaxComboBox->setVisible(false);
    connect(minMaxComboBox,
            SIGNAL(currentIndexChanged(int)),
            SIGNAL(conditionChanged()));

    operationComboBox = new QComboBox();
    operationComboBox->addItems(operationsItems);
    operationComboBox->setFixedWidth(155);
    connect(operationComboBox,
            SIGNAL(currentIndexChanged(int)),
            SIGNAL(conditionChanged()));

    firstValueInput = new QLineEdit();
    firstValueInput->setFixedWidth(50);
    connect(firstValueInput,
            SIGNAL(returnPressed()),
            SIGNAL(conditionChanged()));

    layout = new QGridLayout();
    layout->addWidget(conditionTypeComboBox, 1, 1);
    layout->addWidget(minMaxComboBox, 1, 2);
    layout->addWidget(operationComboBox, 1, 3);
    layout->addWidget(firstValueInput, 1, 4);
    setLayout(layout);
}

QString ConditionWidget::condition() {
    QString cond = translatedMeasurementsItems[conditionTypeComboBox->currentIndex()];

    if (1 <= conditionTypeComboBox->currentIndex() && conditionTypeComboBox->currentIndex() <= 4) {
        QString minMaxText = translatedMinMaxItems[minMaxComboBox->currentIndex()];
        if (minMaxText != "diff") {
            cond = minMaxText + cond;
        }
        else {
            cond = "(max" + cond + " - min" + cond + ")";
        }
    }

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
