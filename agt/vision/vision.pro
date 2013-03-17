QT += gui

TEMPLATE = lib

DEPENDPATH += .
INCLUDEPATH += .

LIBS += -lopencv_core

HEADERS += ipl-image-viewer.h
SOURCES += ipl-image-viewer.cpp

TARGET = agt_vision
headers.files = $$HEADERS

CONFIG(local){
INCLUDEPATH += ../../
}

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/vision
}

INSTALLS += target headers
