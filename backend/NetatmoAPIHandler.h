#ifndef NETATMOAPIHANDLER_H
#define NETATMOAPIHANDLER_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDateTime>


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

    // OTHER

    void currentTimeChanged(QDateTime);

private:
    QNetworkAccessManager *tokensManager;
    QNetworkAccessManager *currentConditionsManager;
    QNetworkAccessManager *dailyRequestManager;
    QString accessToken = "";
    QString refreshToken = "";

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

};

#endif // NETATMOAPIHANDLER_H
