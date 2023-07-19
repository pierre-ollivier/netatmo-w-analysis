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

    std::vector<QVariant> getValues(QString query, int limit = 0);
    std::vector<QVariant> getValuesDates(QString query, int limit = 0);

    QString measurementCapitalizedFromRadioButtons();
    QString operationFromRadioButtons();
    QString unitWithLeadingSpaceFromQuery();
    int decimalsFromQuery();
    QString databaseFromCheckBox();
    QString conditionFromWidgets();

    void displayMoreResults();
    void displayLessResults();

    void selectQueryParams();
    void selectCustomQuery();

    void changeDisplayMonth();

    void sendRequest();
    void showQueryBuilder();

    int maxNumberOfRecords();

    void pasteQueryFromClipboard();

    QString buildQuery(QString databaseName,
                       QString operation,
                       QString measurementCapitalized,
                       QString monthCondition,
                       QString order = "",
                       int limit = 0);
    QString buildDateQuery(QString databaseName,
                           QString operation,
                           QString measurementCapitalized,
                           QString monthCondition,
                           QString order = "",
                           int limit = 0);

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

    const QMap<QString, int> decimalsFromMeasurement = QMap<QString, int>(
    {
                std::pair<QString, int>("temperature", 1),
                std::pair<QString, int>("humidity", 0),
                std::pair<QString, int>("dewpoint", 1),
                std::pair<QString, int>("humidex", 1),
                std::pair<QString, int>("pressure", 1),
                std::pair<QString, int>("co2", 0),
                std::pair<QString, int>("noise", 0),
                std::pair<QString, int>("", 3),
            });
};

#endif // DATAEXPLORATOR_H
