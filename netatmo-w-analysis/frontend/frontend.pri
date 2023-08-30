QT += charts
QT += core

include ($$PWD/PredictionWidget/PredictionWidget.pri)
include ($$PWD/QueryBuilder/QueryBuilder.pri)

HEADERS += \
    $$PWD/ColorUtils.h \
    $$PWD/CustomItemDelegate.h \
    $$PWD/DataExplorator.h \
    $$PWD/EphemerisPanel.h \
    $$PWD/HomePageChart.h \
    $$PWD/MainWindow.h \
    $$PWD/MonthlyReport.h \
    $$PWD/NormalsVisualizer.h \
    $$PWD/YearMonthPicker.h \
    $$PWD/YearlyReport.h

SOURCES += \
    $$PWD/ColorUtils.cpp \
    $$PWD/CustomItemDelegate.cpp \
    $$PWD/DataExplorator.cpp \
    $$PWD/EphemerisPanel.cpp \
    $$PWD/HomePageChart.cpp \
    $$PWD/MainWindow.cpp \
    $$PWD/MonthlyReport.cpp \
    $$PWD/NormalsVisualizer.cpp \
    $$PWD/YearMonthPicker.cpp \
    $$PWD/YearlyReport.cpp
