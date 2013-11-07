QT       -= gui
QT       += sql
CONFIG   += C++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = lib

VERSION = 0.1.2

CONFIG(release, debug|release){
    DESTDIR = ./release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.rcc
    UI_DIR = release/.ui
    BUILD = "release"
    BUILD_SUFFIX = ""
    TARGET = QtzCore
}

CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
    BUILD = "debug"
    BUILD_SUFFIX = "d"
    TARGET = QtzCored
}

DEPENDPATH += .
INCLUDEPATH += .

unix {
    target.path = /usr/lib
    headers.path = /usr/include/qtz/core
    LIBS += -lcrypto++
    LINK_MAJ = ""
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_NAME = libqtz-core
    QMAKE_PKGCONFIG_DESCRIPTION = Qtz Core Library
    QMAKE_PKGCONFIG_PREFIX = $$INSTALLBASE
    QMAKE_PKGCONFIG_LIBDIR = $$target.path
    QMAKE_PKGCONFIG_INCDIR = $$headers.path
    QMAKE_PKGCONFIG_VERSION = $$VERSION
}

win32 {
    LIBS += -lcryptopp562
    target.path = $$INSTALL_ROOT/lib
    headers.path = $$INSTALL_ROOT/include/qtz/core
    LINK_MAJ = "0"
    RC_FILE = QtzCore.rc
}

CONFIG(local){
    INCLUDEPATH += ../../
}

HEADERS += \
    library.h \
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

headers.files = $$HEADERS

INSTALLS += target
INSTALLS += headers

QMAKE_CXXFLAGS += -std=c++0x
