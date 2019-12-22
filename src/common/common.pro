QT += gui-private
TEMPLATE = lib
CONFIG += c++17 static

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
