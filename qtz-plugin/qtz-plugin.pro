CONFIG      += plugin debug_and_release
greaterThan(QT_MAJOR_VERSION, 4): QT += designer
lessThan(QT_MAJOR_VERSION, 5): CONFIG += designer
TARGET      = $$qtLibraryTarget(qtzplugin)
TEMPLATE    = lib

CONFIG(local){
    INCLUDEPATH += ../
    LIBS += -L"../qtz-release/core/release" -lQtzCore -L"../qtz-release/data/release" -lQtzData -L"../qtz-release/widgets/release" -lQtzWidgets
    QT += sql
}

CONFIG      += QTZ
QTZ         += widgets

DEFINES += QTZ_LIBRARY

VERSION = 0.1.0

HEADERS     =   \
    global.h \
    date-query_plugin.h \
    numeric-query_plugin.h \
    text-query_plugin.h \
    table-edit_plugin.h \
    editable-label_plugin.h \
    text-editor-window_plugin.h \
    choose-file_plugin.h \
    qtz.h

SOURCES     =   \
    date-query_plugin.cpp \
    numeric-query_plugin.cpp \
    text-query_plugin.cpp \
    table-edit_plugin.cpp \
    editable-label_plugin.cpp \
    text-editor-window_plugin.cpp \
    choose-file_plugin.cpp \
    qtz.cpp

RESOURCES   = icons.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

OTHER_FILES += \
    qtz_plugin.json

