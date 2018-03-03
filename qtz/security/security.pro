QT += core gui sql xml
CONFIG += c++11

DEFINES += QTZ_SECURITY_LIBRARY

TEMPLATE = lib
VERSION = 0.1.1

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
    DEFINES+=QTZ_PRIVATE_KEY={0x2b,0x5f,0xa3,0x5a,0xa5,0x1e,0xfa,0xc5,0x7e,0x84,0xb6,0x0f,0xb9,0xb4,0x89,0x28}
    DEFINES+=QTZ_INITIALIZATION_VECTOR={0xce,0xaa,0x7c,0x4b,0x4a,0x05,0xc4,0x74,0x72,0xc3,0x6e,0xa2,0x58,0xde,0xf6,0x29}
}

DEPENDPATH += .
INCLUDEPATH += .

unix {
    LIBS += -lssl -lcrypto
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
    LIBS += -llibsslMD -llibcryptoMD
    LIBS += -lIphlpapi
    target.path = $$INSTALL_ROOT/lib
    headers.path = $$INSTALL_ROOT/include/qtz/security
    LINK_MAJ = "0"
    RC_FILE = QtzSecurity.rc
}

CONFIG(local){
    INCLUDEPATH += ../../
    greaterThan(QT_MAJOR_VERSION, 4): {
        QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../core/$$BUILD")
        QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../data/$$BUILD")
    }
    lessThan(QT_MAJOR_VERSION, 5):{
        QMAKE_LIBDIR += "$$OUT_PWD/../core/$$BUILD"
        QMAKE_LIBDIR += "$$OUT_PWD/../data/$$BUILD"
    }
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
}else {
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
}

SOURCES += \
    authentication-source.cpp \
    authenticator.cpp \
    authorizer.cpp \
    identity.cpp \
    password.cpp \
    simple-authenticator.cpp \
    token.cpp \
    username.cpp \
    crypto.cpp \
    unique-machine-id.cpp

HEADERS += \
    authentication-source.hpp \
    authenticator.hpp \
    authorizer.hpp \
    identity.hpp \
    password.hpp \
    simple-authenticator.hpp \
    token.hpp \
    username.hpp \
    qtz-security.hpp \
    crypto.hpp \
    unique-machine-id.hpp

headers.files = $$HEADERS

INSTALLS += target
INSTALLS += headers
