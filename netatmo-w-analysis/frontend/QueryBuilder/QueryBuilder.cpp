#include "QueryBuilder.h"

QueryBuilder::QueryBuilder()
{
    measurementGroupBox = new QGroupBox("Grandeur mesurée");
    tableGroupBox = new QGroupBox("Table");
    conditionGroupBox = new QGroupBox("Conditions");

    temperatureButton = new QPushButton("Température");
    humidityButton = new QPushButton("Humidité");
    dewPointButton = new QPushButton("Point de rosée");
    humidexButton = new QPushButton("Humidex");
    otherButton = new QPushButton("Autre");

    indoorDailyButton = new QPushButton("Intérieur");
    outdoorDailyButton = new QPushButton("Extérieur");

    addConditionButton = new QPushButton("Ajouter...");
    connect(addConditionButton, SIGNAL(clicked()), SLOT(addCondition()));

    conditionWidgets = new QList<ConditionWidget *>();

    measurementGroupBoxLayout = new QGridLayout();
    tableGroupBoxLayout = new QGridLayout();
    conditionGroupBoxLayout = new QGridLayout();

    measurementGroupBoxLayout->addWidget(temperatureButton);
    measurementGroupBoxLayout->addWidget(humidityButton);
    measurementGroupBoxLayout->addWidget(dewPointButton);
    measurementGroupBoxLayout->addWidget(humidexButton);
    measurementGroupBoxLayout->addWidget(otherButton);

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
}
