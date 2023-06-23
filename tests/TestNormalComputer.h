#ifndef TESTNORMALSCOMPUTER_H
#define TESTNORMALSCOMPUTER_H

#include <QObject>

class TestNormalComputer : public QObject
{
    Q_OBJECT

public:
    TestNormalComputer();

private slots:
    void testNormalMeasurementByMovingAverageIndoor();
    void testNormalMeasurementByMovingAverageOutdoor();
    void testStdevMeasurementByMovingAverageIndoor();
    void testStdevMeasurementByMovingAverageOutdoor();
};

#endif // TESTNORMALSCOMPUTER_H
