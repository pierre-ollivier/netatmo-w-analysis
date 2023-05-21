#ifndef YEARLYREPORT_H
#define YEARLYREPORT_H

#include <QWidget>
#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class YearlyReport : public QWidget
{
    Q_OBJECT
public:
    YearlyReport();

public slots:
    void fillBoard();

signals:

private:
    QTableView *mainView;
    QStandardItemModel *mainModel;

    QHBoxLayout *mainLayout;
    QVBoxLayout *optionsLayout;
};

#endif // YEARLYREPORT_H
