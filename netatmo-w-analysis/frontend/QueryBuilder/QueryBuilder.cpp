#include "QueryBuilder.h"
#include <QApplication>
#include <QClipboard>


QueryBuilder::QueryBuilder()
{
    measurementGroupBox = new QGroupBox("Grandeur mesurée");
    tableGroupBox = new QGroupBox("Table");
    conditionGroupBox = new QGroupBox("Conditions");

    measurementGroupBox->setAlignment(Qt::AlignCenter);
    tableGroupBox->setAlignment(Qt::AlignCenter);
    conditionGroupBox->setAlignment(Qt::AlignCenter);

    measurementButtonGroup = new QButtonGroup();
    operationButtonGroup = new QButtonGroup();

    temperatureButton = new QPushButton("Température");
    humidityButton = new QPushButton("Humidité");
    dewPointButton = new QPushButton("Point de rosée");
    humidexButton = new QPushButton("Humidex");
    otherButton = new QPushButton("Autre");

    measurementButtonGroup->addButton(temperatureButton);
    measurementButtonGroup->addButton(humidityButton);
    measurementButtonGroup->addButton(dewPointButton);
    measurementButtonGroup->addButton(humidexButton);


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
    temperatureButton->setChecked(true);
    maximumButton->setChecked(true);

    connect(measurementButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(updateCurrentMeasurement()));
    connect(operationButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(updateCurrentMeasurement()));
    connect(measurementButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(updateQueryTextEdit()));
    connect(operationButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(updateQueryTextEdit()));

    indoorDailyButton = new QPushButton("Intérieur");
    outdoorDailyButton = new QPushButton("Extérieur");

    connect(indoorDailyButton, SIGNAL(clicked()), SLOT(updateQueryTextEdit()));
    connect(outdoorDailyButton, SIGNAL(clicked()), SLOT(updateQueryTextEdit()));

    indoorDailyButton->setCheckable(true);
    outdoorDailyButton->setCheckable(true);

    indoorDailyButton->setAutoExclusive(true);
    outdoorDailyButton->setAutoExclusive(true);

    indoorDailyButton->setChecked(true);

    addConditionButton = new QPushButton("Ajouter...");
    connect(addConditionButton, SIGNAL(clicked()), SLOT(addCondition()));
    removeNullCheckBox = new QCheckBox("Filtrer les valeurs NULL");
    removeNullCheckBox->setChecked(true);
    connect(removeNullCheckBox, SIGNAL(clicked()), SLOT(updateQueryTextEdit()));

    conditionWidgets = new QList<ConditionWidget *>();

    queryTextEdit = new QTextEdit();
    copyQueryButton = new QPushButton("Copier");

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
    conditionGroupBoxLayout->addWidget(removeNullCheckBox, 1001, 1);

    measurementGroupBox->setLayout(measurementGroupBoxLayout);
    tableGroupBox->setLayout(tableGroupBoxLayout);
    conditionGroupBox->setLayout(conditionGroupBoxLayout);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(measurementGroupBox, 1, 1);
    mainLayout->addWidget(tableGroupBox, 1, 2);
    mainLayout->addWidget(conditionGroupBox, 1, 3, 1, 2);
    mainLayout->addWidget(queryTextEdit, 2, 1, 1, 3);
    mainLayout->addWidget(copyQueryButton, 2, 4);

    connect(copyQueryButton, SIGNAL(clicked()), SLOT(copyQueryContentToClipboard()));

    setLayout(mainLayout);

    updateQueryTextEdit();
}

void QueryBuilder::addCondition() {
    ConditionWidget *cwid = new ConditionWidget();
    conditionWidgets->append(cwid);
    updateCurrentMeasurement();
    nextConditionWidgetCoordinate++;

    conditionGroupBoxLayout->addWidget(cwid, nextConditionWidgetCoordinate, 1);
    connect(cwid, SIGNAL(conditionChanged()), SLOT(updateQueryTextEdit()));
    connect(cwid, SIGNAL(deleted()), SLOT(removeCondition()));

    addConditionButton->setEnabled(conditionWidgets->size() < 7);
    updateQueryTextEdit();
}

void QueryBuilder::removeCondition() {
    for (ConditionWidget *cwid : *conditionWidgets) {
        if (cwid->isDeleted()) {
            conditionWidgets->removeOne(cwid);
            conditionGroupBoxLayout->removeWidget(cwid);
            delete cwid;
            addConditionButton->setEnabled(true);
            updateQueryTextEdit();
            break;
        }
    }
}

QString QueryBuilder::query() {
    QString queryFromConditions = this->queryFromConditions();
    if (queryFromConditions == "") return "SELECT " + queryFromMeasurement() + " FROM " + queryFromTable();
    return "SELECT " + queryFromMeasurement() + " FROM " + queryFromTable() + " WHERE " + queryFromConditions;
}

QString QueryBuilder::queryFromMeasurement() {
    if (otherButton->isChecked()) return "*";  // TODO

    if (differenceButton->isChecked()) {
        if (temperatureButton->isChecked()) return "(maxTemperature - minTemperature)";
        if (humidityButton->isChecked()) return "(maxHumidity - minHumidity)";
        if (dewPointButton->isChecked()) return "(maxDewPoint - minDewPoint)";
        if (humidexButton->isChecked()) return "(maxHumidex - minHumidex)";
    }

    else {
        QString operation = maximumButton->isChecked() ? "max" :
                            minimumButton->isChecked() ? "min" :
                                                         "avg";
        if (temperatureButton->isChecked()) return operation + "Temperature";
        if (humidityButton->isChecked()) return operation + "Humidity";
        if (dewPointButton->isChecked()) return operation + "DewPoint";
        if (humidexButton->isChecked()) return operation + "Humidex";
    }

    return "*";
}

QString QueryBuilder::queryFromTable() {
    if (indoorDailyButton->isChecked()) return "IndoorDailyRecords";
    if (outdoorDailyButton->isChecked()) return "OutdoorDailyRecords";
    return "";
}

QString QueryBuilder::queryFromConditions() {
    QString query = "";
    for (ConditionWidget *conditionWidget : *conditionWidgets) {
        query += conditionWidget->condition() + " AND ";
    }
    if (query != "") query = query.left(query.length() - 5);  // remove the last " AND "

    if (removeNullCheckBox->isChecked()) {
        if (query == "") {
            query = queryFromMeasurement() + " IS NOT NULL";
        }
        else {
            query += " AND " + queryFromMeasurement() + " IS NOT NULL";
        }
    }
    return query;
}

void QueryBuilder::updateQueryTextEdit() {
    queryTextEdit->setText(query());
}

void QueryBuilder::updateCurrentMeasurement() {
    /*
     * Update the meaning of CurrentMeasurement in each ConditionWidget.
     */
    QString measurement = temperatureButton->isChecked() ? "Temperature" :
                          humidityButton->isChecked() ? "Humidity" :
                          dewPointButton->isChecked() ? "DewPoint" :
                          humidexButton->isChecked() ? "Humidex" : "";

    for (ConditionWidget *cwid : *conditionWidgets) {
        cwid->setCurrentCondition(measurement, queryFromMeasurement());
    }
}

void QueryBuilder::copyQueryContentToClipboard() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(queryTextEdit->toPlainText());
}
