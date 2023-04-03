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

private:
    QNetworkAccessManager *tokensManager;
    QNetworkAccessManager *currentConditionsManager;
    QString accessToken;

};

#endif // NETATMOAPIHANDLER_H
