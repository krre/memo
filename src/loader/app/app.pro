TEMPLATE = app
TARGET = $$PWD/../../../bin/loader
CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

INCLUDEPATH += $$PWD/../lib $$PWD/../..

LIBS += -L$$PWD/../../../lib/ -lcommon -lloader

SOURCES += \
    main.cpp
