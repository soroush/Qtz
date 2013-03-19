TEMPLATE = subdirs

CONFIG += ordered

HEADERS += global.h

headers.files = $$HEADERS

SUBDIRS += \
    core \
    vision \
    data \
    ui

VERSION = 0.1.0

DEFINES += AGT_LIBRARY

RESOURCES +=

unix {
    headers.path = /usr/include/agt
}

INSTALLS += headers
