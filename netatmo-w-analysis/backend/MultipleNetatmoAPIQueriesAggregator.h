#ifndef MULTIPLENETATMOAPIQUERIESAGGREGATOR_H
#define MULTIPLENETATMOAPIQUERIESAGGREGATOR_H

#include <QObject>
#include <QDate>
#include <QNetworkReply>
#include "APIMonitor.h"
#include "../types/ExtTimestampRecord.h"
#include "../types/IntTimestampRecord.h"

class MultipleNetatmoAPIQueriesAggregator : public QObject
{
    Q_OBJECT

public:
    MultipleNetatmoAPIQueriesAggregator(QObject *parent, APIMonitor *monitor);

    void postOutdoorTimestampRecordsRequest(
        QDate dateBegin,
        QDate dateEnd,
        QString accessToken
        );
public slots:
    void retrieveOutdoorTimestampRecords(QNetworkReply *reply);

signals:
    void outdoorRecordListRetrieved(QDate, QDate, QList<ExtTimestampRecord>);

private:
    QNetworkAccessManager *outdoorRequestManager;
    APIMonitor *apiMonitor;

    QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>> savedRecords;

    int numberOfResponsesReceived = 0;
    int numberOfResponsesToReceive = 0;

    QDate beginDate = QDate();
    QDate endDate = QDate();
};

#endif // MULTIPLENETATMOAPIQUERIESAGGREGATOR_H
