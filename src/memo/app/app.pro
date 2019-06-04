QT += core gui widgets network sql

CONFIG += c++17

TEMPLATE = app
!win32: TARGET = $$PWD/../../../bin/Memo
win32: TARGET = ../../../../bin/Memo

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

win32: RC_ICONS += $$PWD/../lib/images/icon.ico
macx: ICON = $$PWD/../lib/images/icon.icns

INCLUDEPATH += $$PWD/../lib $$PWD/../..

LIBS += -L$$PWD/../../../lib/ -lcommon -lmemo

linux: LIBS += -lX11
win32: LIBS += -luser32

SOURCES += \
    main.cpp

TRANSLATIONS = i18n/memo-ru.ts

RESOURCES += \
    resources.qrc

DISTFILES += \
    i18n/memo-ru.ts
