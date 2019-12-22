QT += core gui widgets network sql
linux: QT += x11extras

TEMPLATE = app
CONFIG += c++17

!win32: TARGET = $$PWD/../../../bin/Memo
win32: TARGET = ../../../../bin/Memo

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

win32: RC_ICONS += $$PWD/../lib/images/icon.ico
macx: ICON = $$PWD/../lib/images/icon.icns

INCLUDEPATH += $$PWD/../lib $$PWD/../..

LIBS += -L$$PWD/../../../lib/ -lmemo -lcommon

linux: LIBS += -lX11
win32: LIBS += -luser32

unix: PRE_TARGETDEPS += $$PWD/../../../lib/libmemo.a
unix: PRE_TARGETDEPS += $$PWD/../../../lib/libcommon.a

SOURCES += \
    main.cpp

TRANSLATIONS = i18n/memo-ru.ts

RESOURCES += \
    resources.qrc

DISTFILES += \
    i18n/memo-ru.ts
