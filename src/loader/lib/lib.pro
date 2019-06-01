CONFIG += c++17
win32: CONFIG += static

TEMPLATE = lib
!win32: TARGET = $$PWD/../../../lib/loader
win32: TARGET = ../../../../lib/loader

INCLUDEPATH += $$PWD/../../

LIBS += -L$$PWD/../../../lib/ -lcommon

SOURCES += \
    Loader.cpp

HEADERS += \
    Loader.h
