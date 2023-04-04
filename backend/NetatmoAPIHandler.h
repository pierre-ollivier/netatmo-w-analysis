#ifndef NETATMOAPIHANDLER_H
#define NETATMOAPIHANDLER_H

#include <QWidget>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>


class NetatmoAPIHandler
{
public:
    NetatmoAPIHandler();
    void postTokensRequest();
    void postCurrentConditionsRequest();
    void postDailyRequest(int dateBegin, QString scale, QString accessToken);

private:
    QNetworkAccessManager *tokensManager;
    QNetworkAccessManager *currentConditionsManager;
    QNetworkAccessManager *dailyRequestManager;
    QString accessToken;

};

#endif // NETATMOAPIHANDLER_H
