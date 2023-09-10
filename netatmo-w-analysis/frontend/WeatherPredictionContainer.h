#ifndef WEATHERPREDICTIONCONTAINER_H
#define WEATHERPREDICTIONCONTAINER_H

#include <QWidget>

class WeatherPredictionContainer : public QWidget
{
    Q_OBJECT
public:
    explicit WeatherPredictionContainer(QWidget *parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;

signals:
    void clicked();
};

#endif // WEATHERPREDICTIONCONTAINER_H
