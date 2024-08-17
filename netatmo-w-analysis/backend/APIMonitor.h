#ifndef APIMONITOR_H
#define APIMONITOR_H

#include <QObject>
#include <queue>

class APIMonitor : public QObject
{
    Q_OBJECT
public:
    APIMonitor(QObject *parent = nullptr);
    APIMonitor(std::queue<int> timestampsQueue);
    APIMonitor(QObject *parent, const APIMonitor &other);

public slots:
    void addTimestamp();
    void addTimestamp(int timestampInms);
    int requestsCountLast10s();
    int requestsCountLasth();
    void printRequestsCounts();

    std::queue<int> getRequestsTimestamps();

private:
    std::queue<int> requestsTimestamps;
};

#endif // APIMONITOR_H
