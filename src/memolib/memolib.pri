QT += gui-private

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/lib/Exception.h \
    $$PWD/lib/ZipCompressor.h \
    $$PWD/lib/MessageHandler.h

SOURCES += \
    $$PWD/lib/Exception.cpp \
    $$PWD/lib/ZipCompressor.cpp
