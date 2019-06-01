QT += gui-private

TEMPLATE = lib
win32: CONFIG += static

!win32: TARGET = $$PWD/../../lib/common
win32: TARGET = ../../../lib/common

HEADERS += \
    Constants.h \
    Exception.h \
    ZipCompressor.h \
    MessageHandler.h

SOURCES += \
    Exception.cpp \
    ZipCompressor.cpp
