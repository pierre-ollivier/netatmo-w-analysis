QT += widgets
QT += network
QT += sql

include (types/types.pri)
include (frontend/frontend.pri)
include (backend/backend.pri)

SOURCES += \
    credentials.cpp \
    dbparams.cpp \
    main.cpp \
    playground.cpp
