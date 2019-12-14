QT += core gui widgets sql network
linux: QT += x11extras

CONFIG += c++17
win32: CONFIG += static

TEMPLATE = lib
!win32: TARGET = $$PWD/../../../lib/memo
win32: TARGET = ../../../../lib/memo

INCLUDEPATH += $$PWD/../../

LIBS += -L$$PWD/../../../lib/ -lcommon

SOURCES += \
    core/Context.cpp \
    core/DatabaseException.cpp \
    database/Patcher.cpp \
    ui/MainWindow.cpp \
    ui/outliner/Outliner.cpp \
    ui/outliner/TreeModel.cpp \
    ui/outliner/TreeItem.cpp \
    database/Database.cpp \
    ui/Editor.cpp \
    ui/Options.cpp \
    ui/hotkey/GlobalHotkey.cpp \
    ui/outliner/NoteProperties.cpp \
    updater/UpdateChecker.cpp \
    updater/NewUpdates.cpp \
    updater/UpdateDownloader.cpp

linux: SOURCES += ui/hotkey/NativeEventFilterX11.cpp
win32: SOURCES += ui/hotkey/NativeEventFilterWin.cpp
mac: SOURCES += ui/hotkey/NativeEventFilterMac.cpp

HEADERS += \
    core/Constants.h \
    core/Context.h \
    core/DatabaseException.h \
    core/Globals.h \
    database/Patcher.h \
    ui/MainWindow.h \
    ui/outliner/Outliner.h \
    ui/outliner/TreeModel.h \
    ui/outliner/TreeItem.h \
    database/Database.h \
    ui/Editor.h \
    ui/Options.h \
    ui/hotkey/NativeEventFilter.h \
    ui/hotkey/GlobalHotkey.h \
    ui/outliner/NoteProperties.h \
    updater/UpdateChecker.h \
    updater/NewUpdates.h \
    updater/UpdateDownloader.h

RESOURCES += \
    resourceslib.qrc
