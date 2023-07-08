#include "QueryBuilder.h"

QueryBuilder::QueryBuilder()
{
    measurementGroupBox = new QGroupBox("Grandeur mesurée");
    tableGroupBox = new QGroupBox("Table");
    conditionGroupBox = new QGroupBox("Conditions");

    measurementGroupBox->setAlignment(Qt::AlignCenter);
    tableGroupBox->setAlignment(Qt::AlignCenter);
    conditionGroupBox->setAlignment(Qt::AlignCenter);

    temperatureButton = new QPushButton("Température");
    humidityButton = new QPushButton("Humidité");
    dewPointButton = new QPushButton("Point de rosée");
    humidexButton = new QPushButton("Humidex");
    otherButton = new QPushButton("Autre");

    temperatureButton->setCheckable(true);
    humidityButton->setCheckable(true);
    dewPointButton->setCheckable(true);
    humidexButton->setCheckable(true);
    otherButton->setCheckable(true);

    temperatureButton->setAutoExclusive(true);
    humidityButton->setAutoExclusive(true);
    dewPointButton->setAutoExclusive(true);
    humidexButton->setAutoExclusive(true);
    otherButton->setAutoExclusive(true);

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
