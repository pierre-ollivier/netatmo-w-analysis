#ifndef OLDDATAUPLOADER_H
#define OLDDATAUPLOADER_H

#include <QString>
#include <QObject>
#include <QDate>
#include "../backend/NetatmoAPIHandler.h"
#include "../types/ExtTimestampRecord.h"
#include "../types/IntTimestampRecord.h"

class OldDataUploader : public QObject
{
    Q_OBJECT

public:
    OldDataUploader(NetatmoAPIHandler *apiHandler, QString accessToken = "");
    void setAccessToken(QString accessToken);

public slots:
    void addDataFromCurrentMonths(QDate beginDate, QDate endDate, bool indoor = false);
    void addExtTimestampRecordToCopyDatabase(ExtTimestampRecord);
    void addIntTimestampRecordToCopyDatabase(IntTimestampRecord);

    void intBatchRetrieved();
    void extBatchRetrieved();

private:
    NetatmoAPIHandler *_apiHandler;
    QString _accessToken;
    QDate _beginDate = QDate();
    QDate _endDate = QDate();
    int numberOfExtBatchesToRetrieve = 0;
    int numberOfIntBatchesToRetrieve = 0;
};

#endif // OLDDATAUPLOADER_H
