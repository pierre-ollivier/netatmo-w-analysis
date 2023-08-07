#include "PredictionItem.h"

PredictionItem::PredictionItem()
{

}

PredictionItem::PredictionItem(QJsonObject jsonObject) {
    _dateTime = QDateTime::fromSecsSinceEpoch(jsonObject["dt"].toInt());
}

QDateTime PredictionItem::dateTime() {return _dateTime;}
