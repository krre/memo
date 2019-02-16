QT += core gui widgets

TARGET = memo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
        main.cpp \
    ui/MainWindow.cpp \
    ui/outliner/Outliner.cpp \
    ui/outliner/TreeModel.cpp \
    ui/outliner/TreeItem.cpp \
    database/Database.cpp

HEADERS += \
    core/Constants.h \
    ui/MainWindow.h \
    ui/outliner/Outliner.h \
    ui/outliner/TreeModel.h \
    ui/outliner/TreeItem.h \
    database/Database.h

RESOURCES += \
    resources.qrc
