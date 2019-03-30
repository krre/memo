QT += core gui widgets sql network gui-private
linux: QT += x11extras

TARGET = memo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

CONFIG += c++17
CONFIG += object_parallel_to_source

TRANSLATIONS = i18n/memo-ru.ts
win32: RC_ICONS += images/icon.ico

linux: LIBS += -lX11
win32: LIBS += -luser32

include(../memolib/memolib.pri)

SOURCES += \
    main.cpp \
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
    core/App.cpp \
    updater/NewUpdates.cpp \
    updater/UpdateDownloader.cpp \
    core/Exception.cpp

linux: SOURCES += ui/hotkey/NativeEventFilterX11.cpp
win32: SOURCES += ui/hotkey/NativeEventFilterWin.cpp
mac: SOURCES += ui/hotkey/NativeEventFilterMac.cpp

HEADERS += \
    core/Constants.h \
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
    core/App.h \
    updater/NewUpdates.h \
    updater/UpdateDownloader.h \
    core/Exception.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    i18n/memo-ru.ts \
    updater/manifest.json.example
