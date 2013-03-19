QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
        LIBS += -L../data/debug -lagt_data
    }
    CONFIG(release){
        LIBS += -L"../data/release" -lagt_data
    }
}

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/ui
}
win32 {
    target.path = C:/mingw/lib
    headers.path = C:/mingw/include/agt/data
}

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
    security/dialog-user-login.cpp \
    data/widget-edit-table.cpp \
    data/dialog-insert-item.cpp \
    data/dialog-edit-item.cpp


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
    security/dialog-user-login.h \
    data/widget-edit-table.h \
    data/dialog-insert-item.h \
    data/dialog-edit-item.h

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
    security/dialog-user-login.ui \
    data/widget-edit-table.ui

TARGET = agt_ui
headers.files = $$HEADERS

INSTALLS += target headers

RESOURCES += \
    resources.qrc

