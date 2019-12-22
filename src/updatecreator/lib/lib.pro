QT += core gui widgets network
CONFIG += c++17 static
TEMPLATE = lib

!win32: TARGET = $$PWD/../../../lib/updatecreator
win32: TARGET = ../../../../lib/updatecreator

INCLUDEPATH += $$PWD/../../

SOURCES += \
    ClearSizeDialog.cpp \
    Form.cpp \
    NetDialog.cpp \
    MainWindow.cpp \
    Outliner.cpp \
    ListModel.cpp \
    NewProjectDialog.cpp \
    ProjectSettings.cpp

HEADERS += \
    ClearSizeDialog.h \
    Form.h \
    MainWindow.h \
    Constants.h \
    NetDialog.h \
    Outliner.h \
    ListModel.h \
    NewProjectDialog.h \
    ProjectSettings.h
