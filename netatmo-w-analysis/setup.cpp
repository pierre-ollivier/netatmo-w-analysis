#include <QColor>
#include <QFont>
#include <QLocale>
#include <QString>

extern const QString VERSION = "3.0.0";
extern const QString RELEASE_DATE = "17/08/2024";
extern const QString APP_PATH = "D:/Mes programmes/netatmo-w-analysis";
extern const QString PATH_TO_PROD_DATABASE = APP_PATH + "/" + "netatmo-w-analysis/netatmo_analysis.db";
extern const QString PATH_TO_COPY_DATABASE = APP_PATH + "/" + "netatmo-w-analysis/netatmo_analysis_copy.db";
extern const QString PATH_TO_REFRESH_TOKEN_FILE = APP_PATH + "/" + "netatmo-w-analysis/refresh_token.txt";
extern const QString PATH_TO_IMAGES_FOLDER = APP_PATH + "/" + "netatmo-w-analysis/images/";

extern const QColor mainBackgroundColor = QColor(231, 216, 243);

extern const QFont defaultTemperatureFont = QFont("Segoe UI", 9);

extern const QLocale LOCALE = QLocale(QLocale::French, QLocale::France);
