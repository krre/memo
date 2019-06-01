QT += testlib network
CONFIG += testcase

INCLUDEPATH += $$PWD/../../src

LIBS += -L$$PWD/../../lib/ -lcommon -lmemo

SOURCES += \
    Test_UpdateChecker.cpp
