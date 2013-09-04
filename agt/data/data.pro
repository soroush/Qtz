QT       -= gui
QT       += sql core
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
    TARGET = agt_data
}

CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
    BUILD = "debug"
    BUILD_SUFFIX = "_d"
    TARGET = agt_data_d
}

DEPENDPATH += .
INCLUDEPATH += .

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/data
    LINK_MAJ = ""
}
win32 {
    target.path = $$INSTALL_ROOT/lib
    headers.path = $$INSTALL_ROOT/include/agt/data
    LINK_MAJ = "0"
}

CONFIG(local){
    INCLUDEPATH += ../../
    LIBS += -L"../core/$$BUILD" -lagt_core$${BUILD_SUFFIX}$${LINK_MAJ}
} else {
    LIBS += -lagt_core$${BUILD_SUFFIX}$${LINK_MAJ}
}

HEADERS += database.h \
    table-node.h \
    data-provider-information.h

SOURCES += database.cpp \
    data-provider-information.cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += resources/mysql_fk_fetch.sql \
    resources/mysql_fetch_tables.sql \
    resources/backup-grammer.txt \
    resources/mysql_fetch_field_count.sql \
    resources/mysql_fetch_all_row_count.sql \
    resources/mysql_fetch_all_rows_procedure.sql

headers.files = $$HEADERS

INSTALLS += target
INSTALLS += headers
