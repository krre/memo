QT += core gui widgets network

TARGET = updatecreator
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

CONFIG += c++17

include(../memolib/memolib.pri)

SOURCES += \
    ClearSizeDialog.cpp \
    Form.cpp \
    NetDialog.cpp \
    main.cpp \
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
