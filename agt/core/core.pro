QT       -= gui
QT       += sql

TEMPLATE = lib

VERSION = 0.1.0

CONFIG(release, debug|release){
    DESTDIR = ./release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.rcc
    UI_DIR = release/.ui
}

CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
}

DEPENDPATH += .
INCLUDEPATH += .

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
    INCLUDEPATH += C:/mingw/include
}

HEADERS += \
    settings.h \
    acl.h \
    auth-provider.h \
    qio.h \
    person.h \
    user.h

SOURCES += \
    settings.cpp \
    acl.cpp \
    auth-provider.cpp \
    qio.cpp \
    person.cpp \
    user.cpp

TARGET = agt_core
headers.files = $$HEADERS

INSTALLS += target headers
