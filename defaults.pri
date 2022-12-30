QT += core gui widgets sql network httpserver gui-private
CONFIG += c++17

INCLUDEPATH += $$PWD/src
SRC_DIR = $$PWD

LIBS += -L../src -lmemo
