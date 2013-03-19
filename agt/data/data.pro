QT       -= gui
QT       += sql core

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
    CONFIG(debug){
        LIBS += -L../core/debug -lagt_core
    }
    CONFIG(release){
        LIBS += -L"../core/release" -lagt_core
    }
}

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/data
}
win32 {
    target.path = C:/mingw/lib
    headers.path = C:/mingw/include/agt/data
}

HEADERS += database.h \
    table-node.h

SOURCES += database.cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += resources/mysql_fk_fetch.sql \
    resources/mysql_fetch_tables.sql \
    resources/backup-grammer.txt \
    resources/mysql_fetch_field_count.sql \
    resources/mysql_fetch_all_row_count.sql \
    resources/mysql_fetch_all_rows_procedure.sql

TARGET = agt_data
headers.files = $$HEADERS

INSTALLS += target headers


