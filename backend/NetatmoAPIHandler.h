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

private:
    QNetworkAccessManager *tokensManager;
};

#endif // NETATMOAPIHANDLER_H
