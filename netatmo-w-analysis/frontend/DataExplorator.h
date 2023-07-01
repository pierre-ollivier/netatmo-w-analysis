#ifndef DATAEXPLORATOR_H
#define DATAEXPLORATOR_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLocale>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QStandardItemModel>
#include <QTableView>
#include "../backend/DatabaseHandler.h"

class DataExplorator : public QWidget
{
    Q_OBJECT

public:
    DataExplorator(DatabaseHandler *dbHandler);

public slots:
    void fillBoards();

    std::vector<QVariant> getValues(
            QString databaseName,
            QString operation,
            QString measurementCapitalized,
            QString monthCondition,
            QString order = "ASC",
            int limit = 0);
    std::vector<QVariant> getValuesDates(
            QString databaseName,
            QString operation,
            QString measurementCapitalized,
            QString monthCondition,
            QString order = "DESC",
            int limit = 0);

    QString measurementCapitalizedFromRadioButtons();
    QString operationFromRadioButtons();
    QString unitWithLeadingSpaceFromRadioButtons();
    QString databaseFromCheckBox();
    QString conditionFromWidgets();

    void displayHeadersFromRadioButtons();

    void displayMoreResults();
    void displayLessResults();

    void changeDisplayMonth();

    int maxNumberOfRecords(bool indoor);

private:
    QGridLayout *layout;

    QLocale *deviceLocale;

    QTableView *mainViewMax;
    QTableView *mainViewMin;
    QStandardItemModel *mainModelMax;
    QStandardItemModel *mainModelMin;

    QComboBox *monthComboBox;

    QHBoxLayout *optionsLayout;
    QGridLayout *operationsLayout;

    QGroupBox *measurementsGroupBox;
    QRadioButton *temperatureRadioButton;
    QRadioButton *humidityRadioButton;
    QRadioButton *dewPointRadioButton;
    QRadioButton *humidexRadioButton;
    QRadioButton *pressureRadioButton;

    QGroupBox *operationsGroupBox;
    QRadioButton *maximumRadioButton;
    QRadioButton *minimumRadioButton;
    QRadioButton *averageRadioButton;
    QRadioButton *differenceRadioButton;

    QPushButton *moreResultsButton;
    QPushButton *lessResultsButton;

    QCheckBox *interiorCheckBox;

    DatabaseHandler *_dbHandler;

    int numberOfResults = 5;
};

#endif // DATAEXPLORATOR_H
