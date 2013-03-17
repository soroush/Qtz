QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

SOURCES += \
    data/data-navigator.cpp \
    data/date-query.cpp \
    data/numeric-query.cpp \
    data/text-query.cpp \
    data/wizard-backup-database.cpp \
    data/dialog-database-config.cpp \
    data/dialog-edit-table.cpp \
    editors/text-editor-window.cpp \
    editors/editable-label.cpp \
    misc/choose-file.cpp \
    security/dialog-user-login.cpp


HEADERS += \
    data/data-navigator.h \
    data/date-query.h \
    data/numeric-query.h \
    data/text-query.h \
    data/wizard-backup-database.h \
    data/dialog-database-config.h \
    data/dialog-edit-table.h \
    editors/text-editor-window.h \
    editors/editable-label.h \
    misc/choose-file.h \
    security/dialog-user-login.h

FORMS += \
    data/data-navigator.ui \
    data/date-query.ui \
    data/numeric-query.ui \
    data/text-query.ui \
    data/wizard-backup-database.ui \
    data/dialog-database-config.ui \
    data/dialog-edit-table.ui \
    editors/text-editor-window.ui \
    editors/editable-label.ui \
    misc/choose-file.ui \
    security/dialog-user-login.ui

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

RESOURCES += \
    resources.qrc

