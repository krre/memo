QT += core gui widgets network sql

CONFIG += c++17

TEMPLATE = app
TARGET = $$PWD/../../../bin/memo

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

win32: RC_ICONS += $$PWD/../lib/images/icon.ico

INCLUDEPATH += $$PWD/../lib $$PWD/../..

LIBS += -L$$PWD/../../../lib/ -lcommon -lmemo
linux: LIBS += -lX11

SOURCES += \
    main.cpp

TRANSLATIONS = i18n/memo-ru.ts

RESOURCES += \
    resources.qrc

DISTFILES += \
    i18n/memo-ru.ts
