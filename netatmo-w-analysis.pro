QT += widgets
QT += network
QT += sql

include (types/types.pri)
include (frontend/frontend.pri)
include (backend/backend.pri)
include (tests/tests.pro)

SOURCES += \
    credentials.cpp \
    dbparams.cpp \
    main.cpp \
    playground.cpp \
    setup.cpp
