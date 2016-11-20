CONFIG      += plugin debug_and_release
greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += designer
lessThan(QT_MAJOR_VERSION, 5): CONFIG += designer
TARGET      = $$qtLibraryTarget(qtzplugin)
TEMPLATE    = lib

unix{
    LINK_MAJ = ""
}
win32{
    LINK_MAJ = "0"
}

CONFIG(release, debug|release){
    BUILD = "release"
    BUILD_SUFFIX = ""
}

CONFIG(debug, debug|release){
    BUILD = "debug"
    BUILD_SUFFIX = "d"
}

CONFIG(local){
    INCLUDEPATH += ../
    LIBS    += -L"../qtz/core/$$BUILD" -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS    += -L"../qtz/data/$$BUILD" -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS    += -L"../qtz/widgets/$$BUILD" -lQtzWidgets$${BUILD_SUFFIX}$${LINK_MAJ}
    QT      += sql gui
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
} else {
    CONFIG  += QTZ
    QTZ     += widgets
    LIBS    += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ} -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ} -lQtzWidgets$${BUILD_SUFFIX}$${LINK_MAJ}
}

DEFINES += QTZ_LIBRARY

VERSION = 0.1.3

HEADERS     =   \
#    table-edit_plugin.h \
    choose-file_plugin.hpp \
    date-query_plugin.hpp \
    editable-label_plugin.hpp \
    global.hpp \
    numeric-query_plugin.hpp \
    qtz.hpp \
    sexagesimal-spinbox_plugin.hpp \
    text-editor-window_plugin.hpp \
    text-query_plugin.hpp

SOURCES     =   \
    qtz.cpp \
    date-query_plugin.cpp \
    numeric-query_plugin.cpp \
    text-query_plugin.cpp \
#    table-edit_plugin.cpp \
    editable-label_plugin.cpp \
    sexagesimal-spinbox_plugin.cpp \
    text-editor-window_plugin.cpp \
    choose-file_plugin.cpp

RESOURCES   = icons.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

OTHER_FILES += \
    qtz_plugin.json

