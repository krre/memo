QT += gui-private

TEMPLATE = lib

TARGET = $$PWD/../../lib/common

HEADERS += \
    Constants.h \
    Exception.h \
    ZipCompressor.h \
    MessageHandler.h

SOURCES += \
    Exception.cpp \
    ZipCompressor.cpp
