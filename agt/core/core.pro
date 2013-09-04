QT       -= gui
QT       += sql
CONFIG   += C++11

TEMPLATE = lib

VERSION = 0.1.0

CONFIG(release, debug|release){
    DESTDIR = ./release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.rcc
    UI_DIR = release/.ui
    BUILD = "release"
    BUILD_SUFFIX = ""
    TARGET = agt_core
}

CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
    BUILD = "debug"
    BUILD_SUFFIX = "_d"
    TARGET = agt_core_d
}

DEPENDPATH += .
INCLUDEPATH += .

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/core
    LIBS += -lcrypto++
    LINK_MAJ = ""
}

win32 {
    LIBS += -lcryptopp562
    target.path = $$INSTALL_ROOT/lib
    headers.path = $$INSTALL_ROOT/include/agt/core
    LINK_MAJ = "0"
}

CONFIG(local){
    INCLUDEPATH += ../../
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

INSTALLS += target
INSTALLS += headers

QMAKE_CXXFLAGS += -std=c++0x
