#include "MultipleNetatmoAPIQueriesAggregator.h"
#include <QTimeZone>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

MultipleNetatmoAPIQueriesAggregator::MultipleNetatmoAPIQueriesAggregator(QObject *parent, APIMonitor *monitor) : QObject(parent)
{
    outdoorRequestManager = new QNetworkAccessManager(this);
    apiMonitor = monitor;

    savedRecords = QPair<QList<ExtTimestampRecord>, QList<IntTimestampRecord>>();

    connect(outdoorRequestManager, SIGNAL(finished(QNetworkReply*)),
            SLOT(retrieveOutdoorTimestampRecords(QNetworkReply*)));
}

void MultipleNetatmoAPIQueriesAggregator::postOutdoorTimestampRecordsRequest(
    QDate dateBegin,
    QDate dateEnd,
    QString accessToken
    ) {
    if (accessToken == "") qDebug() << "Warning: undefined access token in MultipleNetatmoAPIQueriesAggregator";
    if (dateEnd < dateBegin) return;
    extern const QString mainDeviceId;
    extern const QString outdoorModuleId;

    numberOfResponsesToReceive = dateBegin.daysTo(dateEnd) + 1;

    QUrl url("https://api.netatmo.com/api/getmeasure?");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("access_token", accessToken.toUtf8());
    params.addQueryItem("device_id", mainDeviceId);
    params.addQueryItem("module_id", outdoorModuleId);
    params.addQueryItem("scale", "max");
    params.addQueryItem("type", "temperature,humidity");
    params.addQueryItem("optimize", "false");
    params.addQueryItem("real_time", "true");

    // Handle beginDate
    params.addQueryItem(
        "date_begin", QString::number(
            QDateTime(
                dateBegin.addDays(-1),
                QTime(18, 0),
                QTimeZone(QTimeZone::UTC)
                ).toSecsSinceEpoch()
            )
        );
    params.addQueryItem(
        "date_end", QString::number(
            QDateTime(
                dateBegin,
                QTime(23, 59, 59),
                QTimeZone::UTC
                ).toSecsSinceEpoch()
            )
        );
    outdoorRequestManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();

    // Handle all intermediary dates

    for (QDate d = dateBegin.addDays(1); d < dateEnd; d = d.addDays(1)) {
        params.removeAllQueryItems("date_begin");
        params.removeAllQueryItems("date_end");
        params.addQueryItem(
            "date_begin", QString::number(
                QDateTime(
                    d,
                    QTime(0, 0),
                    QTimeZone(QTimeZone::UTC)
                    ).toSecsSinceEpoch()
                )
            );
        params.addQueryItem(
            "date_end", QString::number(
                QDateTime(
                    d,
                    QTime(23, 59, 59),
                    QTimeZone(QTimeZone::UTC)
                    ).toSecsSinceEpoch()
                )
            );
        outdoorRequestManager->post(request, params.query().toUtf8());
        apiMonitor->addTimestamp();
    }

    // Handle endDate

    params.removeAllQueryItems("date_begin");
    params.removeAllQueryItems("date_end");
    params.addQueryItem(
        "date_begin", QString::number(
            QDateTime(
                (dateEnd > dateBegin? dateEnd : dateEnd.addDays(1)),
                QTime(0, 0),
                QTimeZone(QTimeZone::UTC)
                ).toSecsSinceEpoch()
            )
        );
    params.addQueryItem(
        "date_end", QString::number(
            QDateTime(
                dateEnd.addDays(1),
                QTime(6, 0),
                QTimeZone(QTimeZone::UTC)
                ).toSecsSinceEpoch()
            )
        );

    outdoorRequestManager->post(request, params.query().toUtf8());
    apiMonitor->addTimestamp();

}

void MultipleNetatmoAPIQueriesAggregator::retrieveOutdoorTimestampRecords(QNetworkReply *reply) {
    QList<ExtTimestampRecord> recordsList = QList<ExtTimestampRecord>();
    QByteArray bytes = reply->readAll();
    QJsonDocument js = QJsonDocument::fromJson(bytes);
    QJsonObject tb = js["body"].toObject();
    if (bytes.contains("error")) {
        qDebug() << "ERROR with chart request" << bytes;
    }
    else if (bytes.size() >= 1) {
        numberOfResponsesReceived++;
        foreach (const QString &key, tb.keys()) {
            QJsonValue value = tb.value(key);
            double temperature = value[0].toDouble();
            int humidity = int(0.5 + value[1].toDouble());
            recordsList.append(ExtTimestampRecord(key.toLongLong(), temperature, humidity));
        }
        savedRecords.first += recordsList;
        qDebug() << savedRecords.first[0].toString() << "length:" << savedRecords.first.size();

        if (numberOfResponsesReceived == numberOfResponsesToReceive) {
            std::sort(
                savedRecords.first.begin(),
                savedRecords.first.end(),
                [](ExtTimestampRecord &x, ExtTimestampRecord &y){ return x.timestamp() < y.timestamp(); }
                );
            qDebug() << "All data received!" << "\n" << savedRecords.first[0].toString() << "length:" << savedRecords.first.size();
            // emit outdoorRecordListRetrieved(savedRecords.first.sort());
        }
    }
}
