QT += testlib
CONFIG += testcase

INCLUDEPATH += $$PWD/../../src

LIBS += -L$$PWD/../../lib/ -lcommon -lupdatecreator

SOURCES += \
    Test_ListModel.cpp
