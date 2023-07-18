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
#include "../backend/QueryAnalyzer.h"

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

    std::vector<QVariant> getValues(QString query, int limit = 0);
    std::vector<QVariant> getValuesDates(QString query, int limit = 0);

    QString measurementCapitalizedFromRadioButtons();
    QString operationFromRadioButtons();
    QString unitWithLeadingSpaceFromRadioButtons();
    QString databaseFromCheckBox();
    QString conditionFromWidgets();

    void displayHeadersFromRadioButtons();

    void displayMoreResults();
    void displayLessResults();

    void selectQueryParams();
    void selectCustomQuery();

    void changeDisplayMonth();

    void sendRequest();
    void showQueryBuilder();

    int maxNumberOfRecords(bool indoor);

    void pasteQueryFromClipboard();

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
    QPushButton *pasteQueryButton;
    QPushButton *sendQueryButton;
    QPushButton *buildQueryButton;

    QRadioButton *queryParamsSelected;
    QRadioButton *customQuerySelected;

    QPushButton *moreResultsButton;
    QPushButton *lessResultsButton;

    QCheckBox *interiorCheckBox;

    DatabaseHandler *_dbHandler;
    QueryAnalyzer *analyzer;

    int numberOfResults = 5;
    bool lastOperationWasFromCustomQuery = false;

    const QMap<QString, QString> unitFromMeasurement = QMap<QString, QString>(
    {
                std::pair<QString, QString>("temperature", "°C"),
                std::pair<QString, QString>("humidity", "%"),
                std::pair<QString, QString>("dewpoint", "°C"),
                std::pair<QString, QString>("humidex", ""),
                std::pair<QString, QString>("pressure", "hPa"),
                std::pair<QString, QString>("co2", "ppm"),
                std::pair<QString, QString>("noise", "dB"),
                std::pair<QString, QString>("", ""),
            });
};

#endif // DATAEXPLORATOR_H
