#ifndef YEARLYREPORT_H
#define YEARLYREPORT_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class YearlyReport : public QWidget
{
public:
    YearlyReport();

private:
    QTableView *mainView;
    QStandardItemModel *mainModel;

    QHBoxLayout *mainLayout;
    QVBoxLayout *optionsLayout;
};

#endif // YEARLYREPORT_H
