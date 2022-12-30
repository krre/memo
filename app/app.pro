include(../defaults.pri)

TEMPLATE = app
TARGET = memo

linux: LIBS += -lX11
win32: LIBS += -luser32

SOURCES += main.cpp

RESOURCES += \
    ../src/resources.qrc
