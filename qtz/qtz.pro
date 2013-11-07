TEMPLATE = subdirs

CONFIG += ordered
CONFIG += C++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++0x

HEADERS += global.h

headers.files = $$HEADERS

SUBDIRS += \
    core \
#    vision \
    data \
    widgets

VERSION = 0.1.1

DEFINES += QTZ_LIBRARY

unix {
    headers.path = /usr/include/qtz
}
win32 {
    headers.path = $$INSTALL_ROOT/include/qtz/
}

INSTALLS += headers

OTHER_FILES += cxx.astyle
