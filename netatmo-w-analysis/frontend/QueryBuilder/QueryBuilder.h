#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QWidget>
#include "ConditionWidget.h"

class QueryBuilder : public QWidget
{
    Q_OBJECT
public:
    QueryBuilder();

    QString query();
    QString queryFromMeasurement();
    QString queryFromTable();
    QString queryFromConditions();

public slots:
    void addCondition();
    void removeCondition();
    void updateQueryTextEdit();

private:
    QGroupBox *measurementGroupBox;
    QGroupBox *tableGroupBox;
    QGroupBox *conditionGroupBox;

    QPushButton *temperatureButton;
    QPushButton *humidityButton;
    QPushButton *dewPointButton;
    QPushButton *humidexButton;
    QPushButton *otherButton;

    QButtonGroup *measurementButtonGroup;
    QButtonGroup *operationButtonGroup;

    QRadioButton *maximumButton;
    QRadioButton *minimumButton;
    QRadioButton *averageButton;
    QRadioButton *differenceButton;

    QPushButton *indoorDailyButton;
    QPushButton *outdoorDailyButton;

    QPushButton *addConditionButton;

    QTextEdit *queryTextEdit;

    QGridLayout *measurementGroupBoxLayout;
    QGridLayout *tableGroupBoxLayout;
    QGridLayout *conditionGroupBoxLayout;

    QList<ConditionWidget *> *conditionWidgets;

    QGridLayout *mainLayout;

    unsigned int nextConditionWidgetCoordinate = 0;
};

#endif // QUERYBUILDER_H
