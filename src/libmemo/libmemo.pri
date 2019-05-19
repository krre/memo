QT += gui-private

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/memo/Constants.h \
    $$PWD/memo/Exception.h \
    $$PWD/memo/ZipCompressor.h \
    $$PWD/memo/MessageHandler.h

SOURCES += \
    $$PWD/memo/Exception.cpp \
    $$PWD/memo/ZipCompressor.cpp
