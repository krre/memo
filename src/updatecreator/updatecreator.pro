TEMPLATE = app
TARGET = updatecreator

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

CONFIG += c++17

include(updatecreator.pri)

SOURCES += \
    main.cpp
