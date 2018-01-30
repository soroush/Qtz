QT       -= gui
QT       += sql core
CONFIG   += C++11

DEFINES += QTZ_DATA_LIBRARY

TEMPLATE = lib
VERSION = 0.2.1

CONFIG(release, debug|release){
    DESTDIR = ./release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.rcc
    UI_DIR = release/.ui
    BUILD = "release"
    BUILD_SUFFIX = ""
    TARGET = QtzData
}

CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
    BUILD = "debug"
    BUILD_SUFFIX = "d"
    TARGET = QtzDatad
}

DEPENDPATH += .
INCLUDEPATH += .

unix {
    target.path = /usr/lib
    headers.path = /usr/include/qtz/data
    LINK_MAJ = ""
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_NAME = libqtz-data
    QMAKE_PKGCONFIG_DESCRIPTION = Qtz Data Library
    QMAKE_PKGCONFIG_PREFIX = $$INSTALLBASE
    QMAKE_PKGCONFIG_LIBDIR = $$target.path
    QMAKE_PKGCONFIG_INCDIR = $$headers.path
    QMAKE_PKGCONFIG_VERSION = $$VERSION
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
win32 {
    target.path = $$INSTALL_ROOT/lib
    headers.path = $$INSTALL_ROOT/include/qtz/data
    LINK_MAJ = "0"
    RC_FILE = QtzData.rc
}

CONFIG(local){
    INCLUDEPATH += ../../
    QMAKE_LIBDIR += "$$OUT_PWD/../core/$$BUILD"
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
} else {
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
}

HEADERS += \
    database-pool.hpp \
    jalali-table-model.hpp \
    data-provider-information.hpp \
    data-provider.hpp \
    data.hpp \
    i-insert-record.hpp \
    table-node.hpp

SOURCES += \
    data-provider-information.cpp \
    data-provider.cpp \
    i-insert-record.cpp \
    database-pool.cpp \
    jalali-table-model.cpp

RESOURCES += \
    QtzData.qrc

headers.files = $$HEADERS

INSTALLS += target
INSTALLS += headers
