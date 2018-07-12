QT       -= gui
QT       += sql core
CONFIG   += C++11

DEFINES += QTZ_DATA_LIBRARY

TEMPLATE = lib
VERSION = 0.3.0

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
    headers_base.path = /usr/include/qtz/data
    headers_async.path = /usr/include/qtz/data/async
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
    headers_base.path = $$INSTALL_ROOT/include/qtz/data
    headers_async.path = $$INSTALL_ROOT/include/qtz/data/async
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

HEADERS_ASYNC = \
    async/database-connection.hpp \
    async/database-exception.hpp \
    async/model-register.hpp \
    async/query-request.hpp \
    async/query-result.hpp \
    async/query-thread.hpp \
    async/query-worker.hpp \
    async/sql-table-model.hpp

HEADERS_BASE = \
    database-pool.hpp \
    jalali-table-model.hpp \
    data-provider-information.hpp \
    data-provider.hpp \
    data.hpp \
    i-insert-record.hpp \
    table-node.hpp

HEADERS += \
    $$HEADERS_BASE \
    $$HEADERS_ASYNC

SOURCES += \
    data-provider-information.cpp \
    data-provider.cpp \
    i-insert-record.cpp \
    database-pool.cpp \
    jalali-table-model.cpp \
    async/database-connection.cpp \
    async/database-exception.cpp \
    async/model-register.cpp \
    async/query-request.cpp \
    async/query-result.cpp \
    async/query-thread.cpp \
    async/query-worker.cpp \
    async/sql-table-model.cpp


headers_base.files = $$HEADERS_BASE
headers_async.files = $$HEADERS_ASYNC

INSTALLS += target
INSTALLS += \
    headers_base \
    headers_async

RESOURCES += \
    QtzData.qrc
