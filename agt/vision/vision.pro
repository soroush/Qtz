QT += gui

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

unix {
    target.path = /usr/lib
    headers.path = /usr/include/agt/vision
}
win32 {
    target.path = C:/mingw/lib
    headers.path = C:/mingw/include/agt/vision
}

LIBS += -lopencv_core

HEADERS += ipl-image-viewer.h
SOURCES += ipl-image-viewer.cpp

TARGET = agt_vision
headers.files = $$HEADERS

INSTALLS += target headers
