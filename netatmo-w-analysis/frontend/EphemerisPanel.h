#ifndef EPHEMERISPANEL_H
#define EPHEMERISPANEL_H

#include <QGroupBox>
#include <QGridLayout>
#include <QDate>

class EphemerisPanel : public QGroupBox
{
public:
    EphemerisPanel();

    void setDate(QDate date);

private:
    QGridLayout *layout;

};

#endif // EPHEMERISPANEL_H
