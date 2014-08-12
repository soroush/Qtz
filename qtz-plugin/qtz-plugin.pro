CONFIG      += plugin debug_and_release
greaterThan(QT_MAJOR_VERSION, 4): QT += designer CONFIG += C++11
lessThan(QT_MAJOR_VERSION, 5): CONFIG += designer QMAKE_CXXFLAGS += -std=gnu++11
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
    global.h \
    date-query_plugin.h \
    numeric-query_plugin.h \
    text-query_plugin.h \
    table-edit_plugin.h \
    editable-label_plugin.h \
    sexagesimal-spinbox_plugin.h \
    text-editor-window_plugin.h \
    choose-file_plugin.h \
    qtz.h

SOURCES     =   \
    date-query_plugin.cpp \
    numeric-query_plugin.cpp \
    text-query_plugin.cpp \
    table-edit_plugin.cpp \
    editable-label_plugin.cpp \
    sexagesimal-spinbox_plugin.cpp \
    text-editor-window_plugin.cpp \
    choose-file_plugin.cpp \
    qtz.cpp

RESOURCES   = icons.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

OTHER_FILES += \
    qtz_plugin.json
