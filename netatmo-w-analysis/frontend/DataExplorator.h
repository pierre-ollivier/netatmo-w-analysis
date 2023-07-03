#ifndef DATAEXPLORATOR_H
#define DATAEXPLORATOR_H

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLocale>
#include <QPushButton>
#include <QRadioButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>
#include "../backend/DatabaseHandler.h"

class DataExplorator : public QWidget
{
    Q_OBJECT

public:
    DataExplorator(DatabaseHandler *dbHandler);

public slots:
    void fillBoards();
    void fillBoards(QString query);

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

    std::vector<QVariant> getValues(QString query);

    QString measurementCapitalizedFromRadioButtons();
    QString operationFromRadioButtons();
    QString unitWithLeadingSpaceFromRadioButtons();
    QString databaseFromCheckBox();
    QString conditionFromWidgets();

    void displayHeadersFromRadioButtons();

    void displayMoreResults();
    void displayLessResults();

    void changeDisplayMonth();

    void sendRequest();

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
    QGridLayout *customQueryLayout;

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

    QGroupBox *customQueryGroupBox;
    QLineEdit *customQueryLineEdit;
    QPushButton *sendQueryButton;

    QPushButton *moreResultsButton;
    QPushButton *lessResultsButton;

    QCheckBox *interiorCheckBox;

    DatabaseHandler *_dbHandler;

    int numberOfResults = 5;
};

#endif // DATAEXPLORATOR_H
