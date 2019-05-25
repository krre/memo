QT += core gui widgets sql network
linux: QT += x11extras

CONFIG += c++17

TRANSLATIONS = $$PWD/i18n/memo-ru.ts
win32: RC_ICONS += $$PWD/images/icon.ico

linux: LIBS += -lX11
win32: LIBS += -luser32

include(../libmemo/libmemo.pri)

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/ui/MainWindow.cpp \
    $$PWD/ui/outliner/Outliner.cpp \
    $$PWD/ui/outliner/TreeModel.cpp \
    $$PWD/ui/outliner/TreeItem.cpp \
    $$PWD/database/Database.cpp \
    $$PWD/ui/Editor.cpp \
    $$PWD/ui/Options.cpp \
    $$PWD/ui/hotkey/GlobalHotkey.cpp \
    $$PWD/ui/outliner/NoteProperties.cpp \
    $$PWD/updater/UpdateChecker.cpp \
    $$PWD/core/App.cpp \
    $$PWD/updater/NewUpdates.cpp \
    $$PWD/updater/UpdateDownloader.cpp \
    $$PWD/core/SqlException.cpp

linux: SOURCES += $$PWD/ui/hotkey/NativeEventFilterX11.cpp
win32: SOURCES += $$PWD/ui/hotkey/NativeEventFilterWin.cpp
mac: SOURCES += $$PWD/ui/hotkey/NativeEventFilterMac.cpp

HEADERS += \
    $$PWD/core/Constants.h \
    $$PWD/ui/MainWindow.h \
    $$PWD/ui/outliner/Outliner.h \
    $$PWD/ui/outliner/TreeModel.h \
    $$PWD/ui/outliner/TreeItem.h \
    $$PWD/database/Database.h \
    $$PWD/ui/Editor.h \
    $$PWD/ui/Options.h \
    $$PWD/ui/hotkey/NativeEventFilter.h \
    $$PWD/ui/hotkey/GlobalHotkey.h \
    $$PWD/ui/outliner/NoteProperties.h \
    $$PWD/updater/UpdateChecker.h \
    $$PWD/core/App.h \
    $$PWD/updater/NewUpdates.h \
    $$PWD/updater/UpdateDownloader.h \
    $$PWD/core/SqlException.h

RESOURCES += \
    $$PWD/resources.qrc

DISTFILES += \
    $$PWD/i18n/memo-ru.ts
