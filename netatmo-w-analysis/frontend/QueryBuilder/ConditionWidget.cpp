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
    connect(conditionTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            SLOT(changeUnit()));
    connect(conditionTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            SLOT(changeOperationsItems()));

    minMaxComboBox = new QComboBox();
    minMaxComboBox->addItems(minMaxItems);
    minMaxComboBox->setFixedWidth(55);
    minMaxComboBox->setVisible(false);
    connect(minMaxComboBox,
            SIGNAL(currentIndexChanged(int)),
            SIGNAL(conditionChanged()));

    operationComboBox = new QComboBox();
    operationComboBox->addItems(operationsItemsFeminized);
    operationComboBox->setFixedWidth(155);
    connect(operationComboBox,
            SIGNAL(currentIndexChanged(int)),
            SLOT(changeSecondValueInputVisibility()));
    connect(operationComboBox,
            SIGNAL(currentIndexChanged(int)),
            SIGNAL(conditionChanged()));

    firstValueInput = new QLineEdit();
    firstValueInput->setFixedWidth(50);
    connect(firstValueInput,
            SIGNAL(returnPressed()),
            SIGNAL(conditionChanged()));

    secondValueInput = new QLineEdit();
    secondValueInput->setFixedWidth(50);
    secondValueInput->setHidden(true);
    connect(secondValueInput,
            SIGNAL(returnPressed()),
            SIGNAL(conditionChanged()));

    separatorLabel = new QLabel("et");
    separatorLabel->setFixedWidth(15);
    separatorLabel->setAlignment(Qt::AlignCenter);
    separatorLabel->setHidden(true);

    unitLabel = new QLabel();
    unitLabel->setFixedWidth(15);

    deleteButton = new QPushButton("Supprimer");
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(setDeleted()));
    connect(deleteButton, SIGNAL(clicked()), this, SIGNAL(deleted()));

    layout = new QGridLayout();
    layout->addWidget(conditionTypeComboBox, 1, 1);
    layout->addWidget(minMaxComboBox, 1, 2);
    layout->addWidget(operationComboBox, 1, 3);
    layout->addWidget(firstValueInput, 1, 4);
    layout->addWidget(separatorLabel, 1, 5);
    layout->addWidget(secondValueInput, 1, 6);
    layout->addWidget(unitLabel, 1, 7);
    layout->addWidget(deleteButton, 1, 8);
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
    case 6:
        cond += " BETWEEN " + firstValueInput->text() + " AND " + secondValueInput->text();
        break;
    case 7:
        cond += " NOT BETWEEN " + firstValueInput->text() + " AND " + secondValueInput->text();
    }

    cond = cond.replace("&&&", currentConditionFullMeasurement);
    return cond;
}

void ConditionWidget::changeMinMaxComboBoxVisibility() {
    minMaxComboBox->setVisible(1 <= conditionTypeComboBox->currentIndex()
                               && conditionTypeComboBox->currentIndex() <= 4);
}

void ConditionWidget::changeSecondValueInputVisibility() {
    bool visible = operationComboBox->currentIndex() >= 6;
    secondValueInput->setVisible(visible);
    separatorLabel->setVisible(visible);
}

void ConditionWidget::changeUnit() {
    if (conditionTypeComboBox->currentIndex() != 0) {
        unitLabel->setText(unitItems[conditionTypeComboBox->currentIndex()]);
    }
    else {
        if (currentConditionMeasurementCapitalized == "Temperature"
                || currentConditionMeasurementCapitalized == "DewPoint") {
            unitLabel->setText("°C");
        }
        else if (currentConditionMeasurementCapitalized == "Humidity") {
            unitLabel->setText("%");
        }
        else {
            unitLabel->setText("");
        }
    }
}

void ConditionWidget::changeOperationsItems() {
    int index = conditionTypeComboBox->currentIndex();
    if (index == 0 || index == 1 || index == 2 || index == 5 || index == 6) {
        operationComboBox->clear();
        operationComboBox->addItems(operationsItemsFeminized);
    }
    else {
        operationComboBox->clear();
        operationComboBox->addItems(operationsItemsMasculinized);
    }
}

bool ConditionWidget::isDeleted() {
    return _isDeleted;
}

void ConditionWidget::setDeleted() {
    _isDeleted = true;
}

void ConditionWidget::setCurrentCondition(
        QString measurementTypeCapitalized,
        QString minMax,
        QString fullMeasurement) {
    currentConditionMeasurementCapitalized = measurementTypeCapitalized;
    currentConditionMinMax = minMax;
    currentConditionFullMeasurement = fullMeasurement;
}
