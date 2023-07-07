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

signals:

private:
    QComboBox *measurementComboBox;
    QComboBox *dateComboBox;

    QGridLayout *layout;

    const QStringList measurementsItems = {"Grandeur courante",
                                           "Température",
                                           "Humidité",
                                           "Point de rosée",
                                           "Humidex"};

    const QStringList datesItems = {"Date",
                                    "Année",
                                    "Mois",
                                    "Jour"};
};

#endif // CONDITIONWIDGET_H
