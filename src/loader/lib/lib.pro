TEMPLATE = lib
TARGET = $$PWD/../../../lib/loader

INCLUDEPATH += $$PWD/../../

LIBS += -L$$PWD/../../../lib/ -lcommon

SOURCES += \
    Loader.cpp

HEADERS += \
    Loader.h
