TEMPLATE = app
CONFIG += c++17

!win32: TARGET = $$PWD/../../../bin/loader
win32: TARGET = ../../../../bin/loader

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

INCLUDEPATH += $$PWD/../lib $$PWD/../..

LIBS += -L$$PWD/../../../lib/ -lcommon -lloader

SOURCES += \
    main.cpp
