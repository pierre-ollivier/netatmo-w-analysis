#ifndef CONDITIONWIDGET_H
#define CONDITIONWIDGET_H

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class ConditionWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit ConditionWidget(QWidget *parent = nullptr);
    QString condition();

signals:
    void conditionChanged();

private slots:
    void changeMinMaxComboBoxVisibility();

private:
    QComboBox *conditionTypeComboBox;
    QComboBox *operationComboBox;
    QComboBox *minMaxComboBox;

    QLineEdit *firstValueInput;
    QLineEdit *secondValueInput;

    QLabel *separatorLabel;

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

    const QStringList minMaxItems = {"max.", "min.", "moy.", "var."};
    const QStringList translatedMinMaxItems = {"max", "min", "avg", "diff"};
};

#endif // CONDITIONWIDGET_H
