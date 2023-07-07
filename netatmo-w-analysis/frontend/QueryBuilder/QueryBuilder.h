#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QWidget>
#include "ConditionWidget.h"

class QueryBuilder : public QWidget
{
    Q_OBJECT
public:
    QueryBuilder();

public slots:
    void addCondition();

private:
    QGroupBox *measurementGroupBox;
    QGroupBox *tableGroupBox;
    QGroupBox *conditionGroupBox;

    QPushButton *temperatureButton;
    QPushButton *humidityButton;
    QPushButton *dewPointButton;
    QPushButton *humidexButton;
    QPushButton *otherButton;

    QPushButton *indoorDailyButton;
    QPushButton *outdoorDailyButton;

    QPushButton *addConditionButton;

    QGridLayout *measurementGroupBoxLayout;
    QGridLayout *tableGroupBoxLayout;
    QGridLayout *conditionGroupBoxLayout;

    QList<ConditionWidget *> *conditionWidgets;

    QGridLayout *mainLayout;
};

#endif // QUERYBUILDER_H
