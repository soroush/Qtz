CONFIG      += plugin debug_and_release
greaterThan(QT_MAJOR_VERSION, 4): QT += designer
lessThan(QT_MAJOR_VERSION, 5): CONFIG += designer
TARGET      = $$qtLibraryTarget(agtplugin)
TEMPLATE    = lib

QT += sql

DEFINES += AGT_LIBRARY

VERSION = 0.1.0

INCLUDEPATH += ../
INCLUDEPATH += ../agt-release/

HEADERS     =   \
    agt.h global.h \
    date-query_plugin.h \
    numeric-query_plugin.h \
    text-query_plugin.h \
    table-edit_plugin.h \
    editable-label_plugin.h \
    text-editor-window_plugin.h \
    choose-file_plugin.h

SOURCES     =   \
    agt.cpp \
    date-query_plugin.cpp \
    numeric-query_plugin.cpp \
    text-query_plugin.cpp \
    table-edit_plugin.cpp \
    editable-label_plugin.cpp \
    text-editor-window_plugin.cpp \
    choose-file_plugin.cpp

RESOURCES   = icons.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

LIBS += -lagt_ui0

OTHER_FILES += \
    agt_plugin.json

