#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>
#include "types/DailyRecord.h"

class DatabaseHandler
{
public:
    DatabaseHandler(QString pathToDatabase);
    void postDailyRecord(DailyRecord record);

private:
    QString _pathToDatabase;
};

#endif // DATABASEHANDLER_H
