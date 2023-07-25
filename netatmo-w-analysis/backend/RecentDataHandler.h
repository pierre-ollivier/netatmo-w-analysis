#ifndef RECENTDATAHANDLER_H
#define RECENTDATAHANDLER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include "../netatmo-w-analysis/backend/APIMonitor.h"
#include "../netatmo-w-analysis/types/ExtTimestampRecord.h"
#include "../netatmo-w-analysis/types/IntTimestampRecord.h"

class RecentDataHandler : public QObject
{
    Q_OBJECT
public:
    RecentDataHandler(APIMonitor *monitor);
    void setMinTimestamp(int minTimestamp);

public slots:
    void postRequests(int dateBegin, QString scale, QString accessToken);
    void retrieveIndoorChartRequest(QNetworkReply*);
    void retrieveOutdoorChartRequest(QNetworkReply*);

signals:
    void outdoorRecordListRetrieved(QList<ExtTimestampRecord>);
    void indoorRecordListRetrieved(QList<IntTimestampRecord>);
    void recentRecordListRetrieved(QList<ExtTimestampRecord>);

private:
    APIMonitor *apiMonitor;

    QNetworkAccessManager *outdoorChartRequestManager;
    QNetworkAccessManager *indoorChartRequestManager;

    int _minTimestamp = 0;
};

#endif // RECENTDATAHANDLER_H
