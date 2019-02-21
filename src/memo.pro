QT += core gui widgets sql

TARGET = memo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
# Use QMessageLogContext in release build

CONFIG += c++17

SOURCES += \
        main.cpp \
    ui/MainWindow.cpp \
    ui/outliner/Outliner.cpp \
    ui/outliner/TreeModel.cpp \
    ui/outliner/TreeItem.cpp \
    database/Database.cpp \
    ui/Editor.cpp

HEADERS += \
    core/Constants.h \
    ui/MainWindow.h \
    ui/outliner/Outliner.h \
    ui/outliner/TreeModel.h \
    ui/outliner/TreeItem.h \
    database/Database.h \
    ui/Editor.h

RESOURCES += \
    resources.qrc
