QT       -= gui
QT       += sql

TEMPLATE = lib

TARGET = agt_core
headers.files = $$HEADERS

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += local

CONFIG(local){
INCLUDEPATH += ../../
}

unix {

    target.path = /usr/lib
    headers.path = /usr/include/agt/core
}

INSTALLS += target headers

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
