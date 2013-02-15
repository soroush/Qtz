TEMPLATE = subdirs

CONFIG += local ordered

HEADERS += global.h

headers.files = $$HEADERS

SUBDIRS += \
    core \
    vision \
    ui \
    data \

VERSION = 0.1.0

DEFINES += AGT_LIBRARY

RESOURCES +=

unix {
    headers.path = /usr/include/agt
}

INSTALLS += headers
