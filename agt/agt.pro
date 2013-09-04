TEMPLATE = subdirs

CONFIG += ordered
CONFIG += C++11

HEADERS += global.h

headers.files = $$HEADERS

SUBDIRS += \
    core \
#    vision \
    data \
    ui

VERSION = 0.1.0

DEFINES += AGT_LIBRARY

unix {
    headers.path = /usr/include/agt
}
win32 {
    headers.path = $$INSTALL_ROOT/include/agt/
}

INSTALLS += headers
