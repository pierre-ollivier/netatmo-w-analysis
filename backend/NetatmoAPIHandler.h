#ifndef NETATMOAPIHANDLER_H
#define NETATMOAPIHANDLER_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>


class NetatmoAPIHandler: public QObject
{
    Q_OBJECT

public:
    NetatmoAPIHandler();

public slots:
    void postTokensRequest();
    void postCurrentConditionsRequest(QString accessToken);
    void postDailyRequest(int dateBegin, QString scale, QString accessToken);

    void retrieveTokens(QNetworkReply*);
    void retrieveCurrentConditions(QNetworkReply*);

signals:
    void accessTokenChanged(QString);
    void refreshTokenChanged(QString);

    void temperatureChanged(double);
    void minTemperatureChanged(double);
    void maxTemperatureChanged(double);

    void humidityChanged(int);

    void utcTimeChanged(int);
    void minTemperatureTimeChanged(int);
    void maxTemperatureTimeChanged(int);

private:
    QNetworkAccessManager *tokensManager;
    QNetworkAccessManager *currentConditionsManager;
    QNetworkAccessManager *dailyRequestManager;
    QString accessToken = "";
    QString refreshToken = "";

    double currentTemperature = 0.0;
    double currentMinTemperature = 0.0;
    double currentMaxTemperature = 0.0;

    int currentHumidity = 0;

    int currentUTCTime = 0;
    int currentMinTemperatureTime = 0;
    int currentMaxTemperatureTime = 0;

};

#endif // NETATMOAPIHANDLER_H
