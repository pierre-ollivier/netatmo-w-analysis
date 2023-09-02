#include "PredictionWidget.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

PredictionWidget::PredictionWidget(QWidget *parent) : QGroupBox(parent)
{
    maxTemperatureLabel = new QLabel("0 °C");
    minTemperatureLabel = new QLabel("0 °C");
    pictogramLabel = new QLabel();

    layout = new QGridLayout();
    layout->addWidget(maxTemperatureLabel, 1, 1);
    layout->addWidget(minTemperatureLabel, 2, 1);
    layout->addWidget(pictogramLabel, 1, 2, 2, 1);

    setLayout(layout);

    setAlignment(Qt::AlignHCenter);

    setPictogram("");
}

void PredictionWidget::setMaximumTemperature(double maxTemperature) {
    _maxTemperature = maxTemperature;
    maxTemperatureLabel->setText("<font color=\"#ff1000\">↑ </font>"
                                 + QString::number(_maxTemperature, 'f', 0) + " °C");
}

void PredictionWidget::setMinimumTemperature(double minTemperature) {
    _minTemperature = minTemperature;
    minTemperatureLabel->setText("<font color=\"#0010ff\">↓ </font>"
                                 + QString::number(_minTemperature, 'f', 0) + " °C");
}

void PredictionWidget::setPictogram(QString iconId) {
    qDebug() << "Setting pictogram...";
    QUrl imageUrl("http://openweathermap.org/img/wn/10d.png");
    // Create a network access manager to download the image.
    QNetworkAccessManager networkManager;
    QNetworkRequest request(imageUrl);

    QNetworkReply *reply = networkManager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        qDebug() << reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            // Read the image data from the reply.
            QByteArray imageData = reply->readAll();

            // Load the image data into a QPixmap and set it in the QLabel.
            QPixmap pixmap;
            pixmap.loadFromData(imageData);

            if (!pixmap.isNull()) {
                pictogramLabel->setPixmap(pixmap);
                pictogramLabel->setScaledContents(true); // Scale the image to fit the label.
                qDebug() << "Pixmap set.";
            } else {
                qDebug() << "Failed to load image.";
            }
        } else {
            qDebug() << "Network error: " << reply->errorString();
        }

        reply->deleteLater();
    });
}
