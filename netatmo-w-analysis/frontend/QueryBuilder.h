#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <QGridLayout>
#include <QGroupBox>
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

    QGridLayout *measurementGroupBoxLayout;
    QGridLayout *tableGroupBoxLayout;
    QGridLayout *conditionGroupBoxLayout;

    QGridLayout *mainLayout;
};

#endif // QUERYBUILDER_H
