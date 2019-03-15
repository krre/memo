QT += core gui widgets sql network
linux: QT += x11extras

TARGET = memo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

TRANSLATIONS = i18n/memo-ru.ts
win32: RC_ICONS += images/icon.ico

linux: LIBS += -lX11
win32: LIBS += -luser32

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
    core/App.cpp

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
    core/App.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    i18n/memo-ru.ts \
    updater/redirector.json
