QT += core gui widgets network

INCLUDEPATH += $$PWD

include(../libmemo/libmemo.pri)

SOURCES += \
    $$PWD/ClearSizeDialog.cpp \
    $$PWD/Form.cpp \
    $$PWD/NetDialog.cpp \
    $$PWD/MainWindow.cpp \
    $$PWD/Outliner.cpp \
    $$PWD/ListModel.cpp \
    $$PWD/NewProjectDialog.cpp \
    $$PWD/ProjectSettings.cpp

HEADERS += \
    $$PWD/ClearSizeDialog.h \
    $$PWD/Form.h \
    $$PWD/MainWindow.h \
    $$PWD/Constants.h \
    $$PWD/NetDialog.h \
    $$PWD/Outliner.h \
    $$PWD/ListModel.h \
    $$PWD/NewProjectDialog.h \
    $$PWD/ProjectSettings.h
