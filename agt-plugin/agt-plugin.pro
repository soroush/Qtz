CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(agtplugin)
TEMPLATE    = lib

DEFINES += AGT_LIBRARY

VERSION = 0.1.0

INCLUDEPATH += ../
INCLUDEPATH += ../agt-release/

HEADERS     =   \
    agt.h global.h \
    data/date-query_plugin.h \
    data/numeric-query_plugin.h \
    data/text-query_plugin.h \
    editors/editable-label_plugin.h \
    editors/text-editor-window_plugin.h \
    misc/choose-file_plugin.h
SOURCES     =   \
    agt.cpp \
    data/date-query_plugin.cpp \
    data/numeric-query_plugin.cpp \
    data/text-query_plugin.cpp \
    editors/editable-label_plugin.cpp \
    editors/text-editor-window_plugin.cpp \
    misc/choose-file_plugin.cpp

RESOURCES   = icons.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

LIBS += -lagt_ui -lagt_vision

