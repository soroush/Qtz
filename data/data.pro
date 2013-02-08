QT       -= gui
QT       += sql core
TEMPLATE = lib

SOURCES += database.cpp

HEADERS += database.h \
    table-node.h

TARGET = agt_data
headers.files = $$HEADERS

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += local

CONFIG(local){
INCLUDEPATH += ../../
}

unix {

    target.path = /usr/lib
    headers.path = /usr/include/agt/data
}

INSTALLS += target headers

RESOURCES += \
    resources.qrc

OTHER_FILES += resources/mysql_fk_fetch.sql \
    resources/mysql_fetch_tables.sql
