#include "QueryBuilder.h"

QueryBuilder::QueryBuilder()
{
    measurementGroupBox = new QGroupBox("Grandeur mesurée");
    tableGroupBox = new QGroupBox("Table");
    conditionGroupBox = new QGroupBox("Conditions");

    measurementGroupBoxLayout = new QGridLayout();
    tableGroupBoxLayout = new QGridLayout();
    conditionGroupBoxLayout = new QGridLayout();

    measurementGroupBox->setLayout(measurementGroupBoxLayout);
    tableGroupBox->setLayout(tableGroupBoxLayout);
    conditionGroupBox->setLayout(conditionGroupBoxLayout);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(measurementGroupBox, 1, 1);
    mainLayout->addWidget(tableGroupBox, 1, 2);
    mainLayout->addWidget(conditionGroupBox, 1, 3);

    setLayout(mainLayout);
}
