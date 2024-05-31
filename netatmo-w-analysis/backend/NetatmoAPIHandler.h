#ifndef NETATMOAPIHANDLER_H
#define NETATMOAPIHANDLER_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QTimer>
#include <QPointF>
#include "APIMonitor.h"
#include "../types/TimestampRecord.h"
#include "../types/ExtTimestampRecord.h"
#include "../types/IntTimestampRecord.h"
#include "../types/ExtDailyRecord.h"
#include "../types/IntDailyRecord.h"

extern const QString APP_PATH;
extern const QString PATH_TO_REFRESH_TOKEN_FILE;

class NetatmoAPIHandler: public QObject
{
    Q_OBJECT

public:
    NetatmoAPIHandler(APIMonitor *monitor, int timeBetweenRequests = -1);
    NetatmoAPIHandler(NetatmoAPIHandler &other);

public slots:
    void postTokensRequest();
    void postRefreshTokenRequest();
    void postCurrentConditionsRequest();
    void postCurrentConditionsRequest(QString accessToken);
    void postFullOutdoorDailyRequest(int dateBegin, int dateEnd, QString scale, QString accessToken);
    void post3hDailyRequest(int dateBegin, int dateEnd, QString accessToken);
    void postFullIndoorDailyRequest(int dateBegin, int dateEnd, QString scale, QString accessToken);
    void postOutdoorTimestampRecordsRequest(long long dateBegin, long long dateEnd, QString accessToken);
    void postIndoorTimestampRecordsRequest(long long dateBegin, long long dateEnd, QString accessToken);

    void retrieveTokens(QNetworkReply*);
    void retrieveCurrentConditions(QNetworkReply*);
    void retrieveFullDailyOutdoorConditions(QNetworkReply*);
    void retrieveFullDailyIndoorConditions(QNetworkReply*);
    void retrieve3hOutdoorChartRequest(QNetworkReply *);
    void retrieveOutdoorTimestampRecordsRequest(QNetworkReply *);
    void retrieveIndoorTimestampRecordsRequest(QNetworkReply *);

    APIMonitor* getAPIMonitor();
    int getTimeBetweenRequests();
    QString getAccessToken();
    void setAccessToken(QString newAccessToken);

signals:
    // TOKENS
    void accessTokenChanged(QString);
    void refreshTokenChanged(QString);

    // EXT TEMPERATURE

    void extTemperatureChanged(double);
    void extMinTemperatureChanged(double);
    void extMaxTemperatureChanged(double);

    // EXT HUMIDITY

    void extHumidityChanged(int);

    // EXT TIMES

    void extUTCTimeChanged(int);
    void extMinTemperatureTimeChanged(int);
    void extMaxTemperatureTimeChanged(int);

    // INT TEMPERATURE

    void intTemperatureChanged(double);
    void intMinTemperatureChanged(double);
    void intMaxTemperatureChanged(double);

    // INT HUMIDITY

    void intHumidityChanged(int);

    // INT TIMES

    void intUTCTimeChanged(int);
    void intMinTemperatureTimeChanged(int);
    void intMaxTemperatureTimeChanged(int);

    // TIMESTAMP RECORD
    void extTimestampRecordRetrieved(ExtTimestampRecord);
    void intTimestampRecordRetrieved(IntTimestampRecord);

    // CHART SERIES
    void outdoorTemperatureListRetrieved(QList<QPointF>);
    void outdoorHumidityListRetrieved(QList<QPointF>);
    void indoorTemperatureListRetrieved(QList<QPointF>);
    void indoorHumidityListRetrieved(QList<QPointF>);
    void outdoorRecordListRetrieved(QList<ExtTimestampRecord>);
    void indoorRecordListRetrieved(QList<IntTimestampRecord>);

    // OTHER

    void currentTimeChanged(QDateTime);

    void extDailyRecordRetrieved(ExtDailyRecord);
    void intDailyRecordRetrieved(IntDailyRecord);
    void ext3hRecordsRetrieved(QMap<QDate, std::tuple<double, double>>);

private:
    QString askForAuthenticationCode();
    void writeRefreshToken(QString pathToFile, QString refreshToken);
    void checkIfThereIsARefreshToken();

    const QString pathToRefreshTokenFile = PATH_TO_REFRESH_TOKEN_FILE;

    QNetworkAccessManager *tokensManager;
    QNetworkAccessManager *currentConditionsManager;
    QNetworkAccessManager *dailyFullOutdoorRequestManager;
    QNetworkAccessManager *dailyFullIndoorRequestManager;
    QNetworkAccessManager *outdoor3hRequestManager;
    QNetworkAccessManager *outdoorTimestampRecordsRequestManager;
    QNetworkAccessManager *indoorTimestampRecordsRequestManager;

    APIMonitor *apiMonitor;

    QString accessToken = "";
    QString refreshToken = "";

    QTimer *currentConditionsTimer;

    double extCurrentTemperature = 0.0;
    double extCurrentMinTemperature = 0.0;
    double extCurrentMaxTemperature = 0.0;

    int extCurrentHumidity = 0;

    int extCurrentUTCTime = 0;
    int extCurrentMinTemperatureTime = 0;
    int extCurrentMaxTemperatureTime = 0;

    double intCurrentTemperature = 0.0;
    double intCurrentMinTemperature = 0.0;
    double intCurrentMaxTemperature = 0.0;

    int intCurrentHumidity = 0;

    int intCurrentUTCTime = 0;
    int intCurrentMinTemperatureTime = 0;
    int intCurrentMaxTemperatureTime = 0;

    double intCurrentPressure = 0.0;
    int intCurrentCO2 = 0;
    int intCurrentNoise = 0;

    int _timeBetweenRequests = -1;
};

#endif // NETATMOAPIHANDLER_H
