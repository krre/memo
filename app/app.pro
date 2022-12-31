include(../defaults.pri)

TEMPLATE = app
TARGET = memo

LIBS += -L../src -lmemo

linux: LIBS += -lX11
win32: LIBS += -luser32

unix: PRE_TARGETDEPS += $$OUT_PWD/../src/libmemo.a

SOURCES += main.cpp

RESOURCES += \
    ../src/resources.qrc
