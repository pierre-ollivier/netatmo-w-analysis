#ifndef PREDICTIONITEM_H
#define PREDICTIONITEM_H

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

class PredictionItem
{
public:
    PredictionItem();
//    PredictionItem(QJsonDocument jsonDocument);
    PredictionItem(QJsonObject jsonObject);

    QDateTime dateTime();

private:
    QDateTime _dateTime;
};

#endif // PREDICTIONITEM_H
