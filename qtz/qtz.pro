TEMPLATE = subdirs

CONFIG += ordered
CONFIG += C++11

HEADERS += global.h

headers.files = $$HEADERS

SUBDIRS += \
    core \
#    vision \
    data \
    widgets

VERSION = 0.1.0

DEFINES += QTZ_LIBRARY

unix {
    headers.path = /usr/include/qtz
}
win32 {
    headers.path = $$INSTALL_ROOT/include/qtz/
}

INSTALLS += headers