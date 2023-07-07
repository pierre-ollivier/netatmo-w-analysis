#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QWidget>

class QueryBuilder : public QWidget
{
    Q_OBJECT
public:
    QueryBuilder();

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

    QGridLayout *measurementGroupBoxLayout;
    QGridLayout *tableGroupBoxLayout;
    QGridLayout *conditionGroupBoxLayout;

    QGridLayout *mainLayout;
};

#endif // QUERYBUILDER_H
