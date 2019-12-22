CONFIG += c++17 static
TEMPLATE = lib

!win32: TARGET = $$PWD/../../../lib/loader
win32: TARGET = ../../../../lib/loader

INCLUDEPATH += $$PWD/../../

SOURCES += \
    Loader.cpp

HEADERS += \
    Loader.h
