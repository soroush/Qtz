QT       += network sql xml testlib
CONFIG += C++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++0x

TARGET = tst_qtz
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

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


SOURCES +=  tst_authprovider.cpp \
            tst_database.cpp \
    main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += QTZ_LIBRARY

HEADERS += \
    tst_authprovider.h \
    tst_database.h

VERSION = 0.1.0

include(config.pri)

RESOURCES +=
