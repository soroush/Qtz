QT       -= gui
QT       += sql

TEMPLATE = lib

TARGET = agt_core

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += local

CONFIG(local){
INCLUDEPATH += ../../
}

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/core
    LIBS += -lcrypto++
}
win32 {
    LIBS += -lcryptopp5
}


HEADERS += \
    settings.h \
    acl.h \
    auth-provider.h \
    qio.h

SOURCES += \
    settings.cpp \
    acl.cpp \
    auth-provider.cpp \
    qio.cpp

win32 {
INCLUDEPATH += C:/mingw/include
}

headers.files = $$HEADERS
INSTALLS += target headers
