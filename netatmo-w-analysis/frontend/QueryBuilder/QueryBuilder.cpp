#include "QueryBuilder.h"


QueryBuilder::QueryBuilder()
{
    measurementGroupBox = new QGroupBox("Grandeur mesurée");
    tableGroupBox = new QGroupBox("Table");
    conditionGroupBox = new QGroupBox("Conditions");

    measurementGroupBox->setAlignment(Qt::AlignCenter);
    tableGroupBox->setAlignment(Qt::AlignCenter);
    conditionGroupBox->setAlignment(Qt::AlignCenter);

    measurementButtonGroup = new QButtonGroup();

    temperatureButton = new QPushButton("Température");
    humidityButton = new QPushButton("Humidité");
    dewPointButton = new QPushButton("Point de rosée");
    humidexButton = new QPushButton("Humidex");
    otherButton = new QPushButton("Autre");

    measurementButtonGroup->addButton(temperatureButton);
    measurementButtonGroup->addButton(humidityButton);
    measurementButtonGroup->addButton(dewPointButton);
    measurementButtonGroup->addButton(humidexButton);

    operationButtonGroup = new QButtonGroup();

    maximumButton = new QRadioButton("Maximum");
    minimumButton = new QRadioButton("Minimum");
    averageButton = new QRadioButton("Moyenne");
    differenceButton = new QRadioButton("Variation");

    operationButtonGroup->addButton(maximumButton);
    operationButtonGroup->addButton(minimumButton);
    operationButtonGroup->addButton(averageButton);
    operationButtonGroup->addButton(differenceButton);

    temperatureButton->setCheckable(true);
    humidityButton->setCheckable(true);
    dewPointButton->setCheckable(true);
    humidexButton->setCheckable(true);

    indoorDailyButton = new QPushButton("Intérieur");
    outdoorDailyButton = new QPushButton("Extérieur");

    indoorDailyButton->setCheckable(true);
    outdoorDailyButton->setCheckable(true);

    indoorDailyButton->setAutoExclusive(true);
    outdoorDailyButton->setAutoExclusive(true);

    addConditionButton = new QPushButton("Ajouter...");
    connect(addConditionButton, SIGNAL(clicked()), SLOT(addCondition()));

    conditionWidgets = new QList<ConditionWidget *>();

    measurementGroupBoxLayout = new QGridLayout();
    tableGroupBoxLayout = new QGridLayout();
    conditionGroupBoxLayout = new QGridLayout();

    measurementGroupBoxLayout->addWidget(temperatureButton, 1, 1);
    measurementGroupBoxLayout->addWidget(humidityButton, 2, 1);
    measurementGroupBoxLayout->addWidget(dewPointButton, 3, 1);
    measurementGroupBoxLayout->addWidget(humidexButton, 4, 1);

    measurementGroupBoxLayout->addWidget(maximumButton, 1, 2);
    measurementGroupBoxLayout->addWidget(minimumButton, 2, 2);
    measurementGroupBoxLayout->addWidget(averageButton, 3, 2);
    measurementGroupBoxLayout->addWidget(differenceButton, 4, 2);

    measurementGroupBoxLayout->addWidget(otherButton, 5, 1, 1, 2);

    tableGroupBoxLayout->addWidget(indoorDailyButton);
    tableGroupBoxLayout->addWidget(outdoorDailyButton);

    conditionGroupBoxLayout->addWidget(addConditionButton, 1000, 1);

    measurementGroupBox->setLayout(measurementGroupBoxLayout);
    tableGroupBox->setLayout(tableGroupBoxLayout);
    conditionGroupBox->setLayout(conditionGroupBoxLayout);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(measurementGroupBox, 1, 1);
    mainLayout->addWidget(tableGroupBox, 1, 2);
    mainLayout->addWidget(conditionGroupBox, 1, 3);

    setLayout(mainLayout);
}

void QueryBuilder::addCondition() {
    int n = conditionWidgets->size();
    conditionWidgets->append(new ConditionWidget());

    conditionGroupBoxLayout->addWidget(conditionWidgets->at(n), n, 1);

    if (n >= 7) {
        addConditionButton->setDisabled(true);
    }
}

QString QueryBuilder::query() {
    return "SELECT " + queryFromMeasurement() + " FROM " + queryFromTable() + " WHERE " + queryFromConditions();
}

QString QueryBuilder::queryFromMeasurement() {
    // TODO: add min, max, avg...
    if (temperatureButton->isChecked()) return "Temperature";
    if (humidityButton->isChecked()) return "Humidity";
    if (dewPointButton->isChecked()) return "Dew point";
    if (humidexButton->isChecked()) return "Humidex";
    if (otherButton->isChecked()) return "*";  // TODO
    return "*";
}

QString QueryBuilder::queryFromTable() {
    if (indoorDailyButton->isChecked()) return "IndoorDailyRecords";
    if (outdoorDailyButton->isChecked()) return "OutdoorDailyRecords";
    return "";
}

QString QueryBuilder::queryFromConditions() {
    return "";  // TODO
}
