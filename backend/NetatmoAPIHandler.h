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
    void postCurrentConditionsRequest();
    void postDailyRequest(int dateBegin, QString scale, QString accessToken);

    void retrieveTokens(QNetworkReply*);

signals:
    void accessTokenChanged(QString);
    void refreshTokenChanged(QString);

private:
    QNetworkAccessManager *tokensManager;
    QNetworkAccessManager *currentConditionsManager;
    QNetworkAccessManager *dailyRequestManager;
    QString accessToken = "";
    QString refreshToken = "";

};

#endif // NETATMOAPIHANDLER_H
