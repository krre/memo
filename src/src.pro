include(../defaults.pri)

TEMPLATE = lib
TARGET = memo
CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

win32: RC_ICONS += $$PWD/images/icon.ico
macx: ICON = $$PWD/images/icon.icns

SOURCES += \
    core/DatabaseException.cpp \
    core/Exception.cpp \
    core/Settings.cpp \
    database/Patcher.cpp \
    server/HttpServer.cpp \
    ui/MainWindow.cpp \
    ui/Preferences.cpp \
    ui/notetaking/NoteTaking.cpp \
    ui/notetaking/TreeModel.cpp \
    ui/notetaking/TreeItem.cpp \
    database/Database.cpp \
    ui/Editor.cpp \
    ui/hotkey/GlobalHotkey.cpp \
    ui/notetaking/NoteProperties.cpp

linux: SOURCES += ui/hotkey/NativeEventFilterX11.cpp
win32: SOURCES += ui/hotkey/NativeEventFilterWin.cpp
mac: SOURCES += ui/hotkey/NativeEventFilterMac.cpp

HEADERS += \
    core/Constants.h \
    core/DatabaseException.h \
    core/Exception.h \
    core/Globals.h \
    core/MessageHandler.h \
    core/Settings.h \
    database/Patcher.h \
    server/HttpServer.h \
    ui/MainWindow.h \
    ui/Preferences.h \
    ui/notetaking/NoteTaking.h \
    ui/notetaking/TreeModel.h \
    ui/notetaking/TreeItem.h \
    database/Database.h \
    ui/Editor.h \
    ui/hotkey/NativeEventFilter.h \
    ui/hotkey/GlobalHotkey.h \
    ui/notetaking/NoteProperties.h

TRANSLATIONS = i18n/memo-ru.ts

DISTFILES += \
    ../README.md \
    i18n/memo-ru.ts
