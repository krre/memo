QT += core gui widgets network
TEMPLATE = app
!win32: TARGET = $$PWD/../../../bin/updatecreator
win32: TARGET = ../../../../bin/updatecreator

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

INCLUDEPATH += $$PWD/../lib $$PWD/../..

LIBS += -L$$PWD/../../../lib/ -lcommon -lupdatecreator

SOURCES += \
    main.cpp
