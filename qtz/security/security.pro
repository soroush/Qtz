QT += core gui sql xml
CONFIG   += C++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

DEFINES += QTZ_SECURITY_LIBRARY

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
    TARGET = QtzSecurity
}

CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
    BUILD = "debug"
    BUILD_SUFFIX = "d"
    TARGET = QtzSecurityd
}

default_key {
    DEFINES+=PRIVATE_KEY=\\\"f9bf8a579d1fa38c4e20ee884e4096c054b57771153799f49fb674d1006caf52\\\"
}

DEPENDPATH += .
INCLUDEPATH += .

unix {
    target.path = /usr/lib
    headers.path = /usr/include/qtz/security
    LINK_MAJ = ""
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_NAME = libqtz-security
    QMAKE_PKGCONFIG_DESCRIPTION = Qtz Security Library
    QMAKE_PKGCONFIG_PREFIX = $$INSTALLBASE
    QMAKE_PKGCONFIG_LIBDIR = $$target.path
    QMAKE_PKGCONFIG_INCDIR = $$headers.path
    QMAKE_PKGCONFIG_VERSION = $$VERSION
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
win32 {
    LIBS += -llibeay32MD
    target.path = $$INSTALL_ROOT/lib
    headers.path = $$INSTALL_ROOT/include/qtz/security
    LINK_MAJ = "0"
    RC_FILE = QtzSecurity.rc
}

CONFIG(local){
    INCLUDEPATH += ../../
    QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../core/$$BUILD")
    QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../data/$$BUILD")
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
}else {
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
}

SOURCES += \
    authentication-token.cpp \
    authentication-source.cpp \
    authenticator.cpp \
    authorization-adapter.cpp \
    authorizer.cpp \
    identity.cpp \
    password.cpp \
    simple-authenticator.cpp \
    token.cpp \
    username.cpp \
    crypto.cpp

HEADERS += \
    authentication-token.hpp \
    authentication-source.hpp \
    authenticator.hpp \
    authorization-adapter.hpp \
    authorizer.hpp \
    identity.hpp \
    password.hpp \
    simple-authenticator.hpp \
    token.hpp \
    username.hpp \
    qtz-security.hpp \
    crypto.hpp

headers.files = $$HEADERS

INSTALLS += target
INSTALLS += headers