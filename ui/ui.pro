QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

SOURCES += \
    data-navigator.cpp \
    date-query.cpp \
    numeric-query.cpp \
    text-editor-window.cpp \
    editable-label.cpp \
    text-query.cpp \
    choose-file.cpp \
    wizard-backup-database.cpp \
    dialog-database-config.cpp


HEADERS += \
    data-navigator.h \
    date-query.h \
    numeric-query.h \
    text-editor-window.h \
    editable-label.h \
    text-query.h \
    choose-file.h \
    wizard-backup-database.h \
    dialog-database-config.h

FORMS += \
    data-navigator.ui \
    date-query.ui \
    numeric-query.ui \
    text-editor-window.ui \
    editable-label.ui \
    text-query.ui \
    choose-file.ui \
    wizard-backup-database.ui \
    dialog-database-config.ui

TARGET = agt_ui
headers.files = $$HEADERS

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += local

CONFIG(local){
INCLUDEPATH += ../../
}

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/ui
}

INSTALLS += target headers

