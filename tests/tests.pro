QT += testlib
QT += gui
QT += widgets
QT += network
QT += sql
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

include (../netatmo-w-analysis/types/types.pri)
include (../netatmo-w-analysis/frontend/frontend.pri)
include (../netatmo-w-analysis/backend/backend.pri)

SOURCES += \
    TestDailyAverageCalculator.cpp \
    TestDailyStatisticsCalculator.cpp \
    TestDatabaseHandler.cpp \
    TestNormalComputer.cpp \
    TestQueryAnalyzer.cpp \
    TestTypes.cpp \
    credentials_test.cpp \
    dbparams_test.cpp \
    main.cpp \
    setupTests.cpp

HEADERS += \
    TestDailyAverageCalculator.h \
    TestDailyStatisticsCalculator.h \
    TestDatabaseHandler.h \
    TestNormalComputer.h \
    TestQueryAnalyzer.h \
    TestTypes.h
