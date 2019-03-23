QT += core gui widgets sql

TARGET = updatecreator
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h
