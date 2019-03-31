QT += core gui widgets

TARGET = updatecreator
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

include(../memolib/memolib.pri)

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Outliner.cpp \
    Form.cpp \
    ListModel.cpp \
    Builder.cpp \
    NewProjectDialog.cpp \
    ProjectSettings.cpp

HEADERS += \
    MainWindow.h \
    Constants.h \
    Outliner.h \
    Form.h \
    ListModel.h \
    Builder.h \
    NewProjectDialog.h \
    ProjectSettings.h
