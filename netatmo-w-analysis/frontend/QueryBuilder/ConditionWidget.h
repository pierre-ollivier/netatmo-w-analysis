#ifndef CONDITIONWIDGET_H
#define CONDITIONWIDGET_H

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QWidget>

class ConditionWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ConditionWidget(QWidget *parent = nullptr);
    QString condition();

signals:

private:
    QComboBox *conditionTypeComboBox;
    QComboBox *operationComboBox;

    QLineEdit *firstValueInput;

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

    const QStringList operationsItems = {"Égal à",
                                         "Différent de",
                                         "Supérieur à",
                                         "Supérieur ou égal à",
                                         "Inférieur à",
                                         "Inférieur ou égal à",
                                         "Compris entre",
                                         "Non compris entre"};
};

#endif // CONDITIONWIDGET_H
