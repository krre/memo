QT += core gui widgets sql
linux: QT += x11extras

TARGET = memo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

TRANSLATIONS = i18n/memo-ru.ts

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
    ui/hotkey/NativeEventFilterMac.cpp

linux: SOURCES += ui/hotkey/NativeEventFilterX11.cpp
win32: SOURCES += ui/hotkey/NativeEventFilterWin.cpp

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
    ui/hotkey/GlobalHotkey.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    i18n/memo-ru.ts
