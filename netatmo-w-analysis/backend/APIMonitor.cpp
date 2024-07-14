#include "APIMonitor.h"
#include <QDateTime>
#include <QDebug>

APIMonitor::APIMonitor(QObject *parent) : QObject(parent)
{
    requestsTimestamps = std::queue<int>();
}

APIMonitor::APIMonitor(std::queue<int> timestampsQueue) {
    requestsTimestamps = timestampsQueue;
}

APIMonitor::APIMonitor(QObject *parent, const APIMonitor &other) : QObject(parent) {
    requestsTimestamps = other.requestsTimestamps;
}

void APIMonitor::addTimestamp() {
    addTimestamp(QDateTime::currentMSecsSinceEpoch());
}

void APIMonitor::addTimestamp(int timestampInms) {
    requestsTimestamps.push(timestampInms);
}

int APIMonitor::requestsCountLasth() {
    // Since we don't need timestamps from more than one hour ago, we delete them
    // to improve the overall performance
    int currentTimestamp = QDateTime::currentMSecsSinceEpoch();
    while (requestsTimestamps.size() > 0 && requestsTimestamps.front() + 3600 * 1000 < currentTimestamp) {
        requestsTimestamps.pop();
    }
    return requestsTimestamps.size();
}

int APIMonitor::requestsCountLast10s() {
    std::queue<int> requestsTimestampsCopy = requestsTimestamps;
    int currentTimestamp = QDateTime::currentMSecsSinceEpoch();
    while (requestsTimestampsCopy.size() > 0 && requestsTimestampsCopy.front() + 10 * 1000 < currentTimestamp) {
        requestsTimestampsCopy.pop();
    }
    return requestsTimestampsCopy.size();
}

void APIMonitor::printRequestsCounts() {
    qDebug() << "Date:" << QDateTime::currentDateTime().toString(Qt::TextDate) << "\n"
             << "Total requests in the last 10 seconds:" << requestsCountLast10s() << "\n"
             << "Total requests in the last hour:" << requestsCountLasth();
}

std::queue<int> APIMonitor::getRequestsTimestamps() {
    return requestsTimestamps;
}
