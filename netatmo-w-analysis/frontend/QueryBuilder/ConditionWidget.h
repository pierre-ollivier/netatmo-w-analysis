#ifndef CONDITIONWIDGET_H
#define CONDITIONWIDGET_H

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class ConditionWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ConditionWidget(QWidget *parent = nullptr);
    QString condition();

public slots:
    bool isDeleted();
    void setDeleted();
    void setCurrentCondition(
            QString measurementTypeCapitalized,
            QString minMax,
            QString fullMeasurement);

signals:
    void conditionChanged();
    void deleted();

private slots:
    void changeMinMaxComboBoxVisibility();
    void changeSecondValueInputVisibility();
    void changeUnit();
    void changeOperationsItems();

private:
    QComboBox *conditionTypeComboBox;
    QComboBox *operationComboBox;
    QComboBox *minMaxComboBox;

    QLineEdit *firstValueInput;
    QLineEdit *secondValueInput;

    QLabel *separatorLabel;
    QLabel *unitLabel;

    QPushButton *deleteButton;

    QGridLayout *layout;

    const QStringList measurementsItems = {"Grandeur courante",
                                           "Température",
                                           "Humidité",
                                           "Point de rosée",
                                           "Humidex"};

    const QStringList datesItems = {"Date",
                                    "Année",
                                    "Mois",
                                    "Jour",
                                    "Numéro de décade",
                                    "Numéro de semaine"};

    const QStringList translatedMeasurementsItems = {"&&&",
                                                     "Temperature",
                                                     "Humidity",
                                                     "DewPoint",
                                                     "Humidex",
                                                     "date",
                                                     "year",
                                                     "month",
                                                     "day",
                                                     "decade",
                                                     "weekNumber"};

    const QStringList operationsItemsMasculinized = {"Égal à",
                                                     "Différent de",
                                                     "Supérieur à",
                                                     "Supérieur ou égal à",
                                                     "Inférieur à",
                                                     "Inférieur ou égal à",
                                                     "Compris entre",
                                                     "Non compris entre"};

    const QStringList operationsItemsFeminized = {"Égale à",
                                                  "Différente de",
                                                  "Supérieure à",
                                                  "Supérieure ou égale à",
                                                  "Inférieure à",
                                                  "Inférieure ou égale à",
                                                  "Comprise entre",
                                                  "Non comprise entre"};

    const QStringList unitItems = {"&&&",
                                   "°C",
                                   "%",
                                   "°C",
                                   "",
                                   "",
                                   "",
                                   "",
                                   "",
                                   "",
                                   ""};

    const QStringList minMaxItems = {"max.", "min.", "moy.", "var."};
    const QStringList translatedMinMaxItems = {"max", "min", "avg", "diff"};

    QString currentConditionMeasurementCapitalized = "Temperature";
    QString currentConditionMinMax = "max";
    QString currentConditionFullMeasurement = "maxTemperature";
    // do we really need currentConditionMeasurementCapitalized and currentConditionMinMax?
    // maybe we could pass the unit and genre instead, to comply with a future custom measurement

    bool _isDeleted = false;
};

#endif // CONDITIONWIDGET_H
