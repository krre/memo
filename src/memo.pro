QT += core gui widgets sql

TARGET = memo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
# Use QMessageLogContext in release build

CONFIG += c++17

TRANSLATIONS = i18n/memo-ru.ts

SOURCES += \
        main.cpp \
    ui/MainWindow.cpp \
    ui/outliner/Outliner.cpp \
    ui/outliner/TreeModel.cpp \
    ui/outliner/TreeItem.cpp \
    database/Database.cpp \
    ui/Editor.cpp \
    ui/Options.cpp

HEADERS += \
    core/Constants.h \
    ui/MainWindow.h \
    ui/outliner/Outliner.h \
    ui/outliner/TreeModel.h \
    ui/outliner/TreeItem.h \
    database/Database.h \
    ui/Editor.h \
    ui/Options.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    i18n/memo-ru.ts
