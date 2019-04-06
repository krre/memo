QT += core gui widgets network

TARGET = updatecreator
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

include(../memolib/memolib.pri)

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Outliner.cpp \
    ListModel.cpp \
    Builder.cpp \
    NewProjectDialog.cpp \
    ProjectSettings.cpp \
    Manifest.cpp \
    UploadDialog.cpp

HEADERS += \
    MainWindow.h \
    Constants.h \
    Outliner.h \
    ListModel.h \
    Builder.h \
    NewProjectDialog.h \
    ProjectSettings.h \
    Manifest.h \
    UploadDialog.h
