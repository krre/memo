include(../../defaults.pri)

QT += testlib
TEMPLATE = app
CONFIG += console

LIBS += -L../../src -lmemo

unix: PRE_TARGETDEPS += $$OUT_PWD/../../src/libmemo.a

SOURCES +=  \
    tst_preferences.cpp

HEADERS +=
