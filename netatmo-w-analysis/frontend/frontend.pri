QT += charts
QT += core

include ($$PWD/PredictionWidget/PredictionWidget.pri)
include ($$PWD/QueryBuilder/QueryBuilder.pri)

HEADERS += \
    $$PWD/ColorUtils.h \
    $$PWD/CumulativeChart.h \
    $$PWD/CustomItemDelegate.h \
    $$PWD/DataExplorator.h \
    $$PWD/EphemerisPanel.h \
    $$PWD/HomePageChart.h \
    $$PWD/MainWindow.h \
    $$PWD/MonthlyReport.h \
    $$PWD/NormalsVisualizer.h \
    $$PWD/PredictionWindow.h \
    $$PWD/WeatherPredictionContainer.h \
    $$PWD/YearMonthPicker.h \
    $$PWD/YearlyReport.h

SOURCES += \
    $$PWD/ColorUtils.cpp \
    $$PWD/CumulativeChart.cpp \
    $$PWD/CustomItemDelegate.cpp \
    $$PWD/DataExplorator.cpp \
    $$PWD/EphemerisPanel.cpp \
    $$PWD/HomePageChart.cpp \
    $$PWD/MainWindow.cpp \
    $$PWD/MonthlyReport.cpp \
    $$PWD/NormalsVisualizer.cpp \
    $$PWD/PredictionWindow.cpp \
    $$PWD/WeatherPredictionContainer.cpp \
    $$PWD/YearMonthPicker.cpp \
    $$PWD/YearlyReport.cpp
