QT += testlib
CONFIG += testcase

INCLUDEPATH += $$PWD/../../src

LIBS += -L$$PWD/../../lib/ -lcommon -lmemo

SOURCES += \
    Test_ZipCompressor.cpp
