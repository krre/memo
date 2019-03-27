QT += core gui widgets sql

TARGET = updatecreator
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Outliner.cpp \
    Form.cpp \
    ListModel.cpp \
    Builder.cpp

HEADERS += \
    MainWindow.h \
    Constants.h \
    Outliner.h \
    Form.h \
    ListModel.h \
    Builder.h
