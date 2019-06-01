QT += testlib
CONFIG += testcase

INCLUDEPATH += $$PWD/../../src

LIBS += -L$$PWD/../../lib/ -lcommon -lloader

SOURCES += \
    Test_Loader.cpp
