QT += core gui sql xml
CONFIG   += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

DEFINES += QTZ_WIDGETS_LIBRARY

TEMPLATE = lib
VERSION = 0.1.3

CONFIG(release, debug|release){
    DESTDIR = ./release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.rcc
    UI_DIR = release/.ui
    BUILD = "release"
    BUILD_SUFFIX = ""
    TARGET = QtzWidgets
}

CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
    BUILD = "debug"
    BUILD_SUFFIX = "d"
    TARGET = QtzWidgetsd
}

DEPENDPATH += .
INCLUDEPATH += .

unix {
    target.path = /usr/lib
    headers_data.path = /usr/include/qtz/widgets/data
    headers_editors.path = /usr/include/qtz/widgets/editors
    headers_misc.path = /usr/include/qtz/widgets/misc
    headers_security.path = /usr/include/qtz/widgets/security
    LINK_MAJ = ""
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_NAME = libqtz-widgets
    QMAKE_PKGCONFIG_DESCRIPTION = Qtz Widgets Library
    QMAKE_PKGCONFIG_PREFIX = $$INSTALLBASE
    QMAKE_PKGCONFIG_LIBDIR = $$target.path
    QMAKE_PKGCONFIG_INCDIR = $$headers.path
    QMAKE_PKGCONFIG_VERSION = $$VERSION
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
win32 {
    target.path = $$INSTALL_ROOT/lib
    headers_data.path = $$INSTALL_ROOT/include/qtz/widgets/data
    headers_editors.path = $$INSTALL_ROOT/include/qtz/widgets/editors
    headers_misc.path = $$INSTALL_ROOT/include/qtz/widgets/misc
    headers_security.path = $$INSTALL_ROOT/include/qtz/widgets/security
    LINK_MAJ = "0"
    RC_FILE = QtzWidgets.rc
}

CONFIG(local){
    INCLUDEPATH += ../../
    QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../core/$$BUILD")
    QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../data/$$BUILD")
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
}else {
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
}

SOURCES += \
    data/data-navigator.cpp \
    data/date-query.cpp \
    data/numeric-query.cpp \
    data/text-query.cpp \
    data/wizard-backup-database.cpp \
    data/dialog-database-config.cpp \
    editors/text-editor-window.cpp \
    editors/editable-label.cpp \
    editors/sexagesimal-spinbox.cpp \
    misc/choose-file.cpp \
    misc/color-button.cpp \
    security/dialog-user-login.cpp \
    data/widget-edit-table.cpp \
    data/dialog-insert-item.cpp \
    data/dialog-edit-item.cpp \
    data/dialog-edit-table.cpp \
    data/wizard-create-database.cpp \
    data/wizard-page-create-database-intro.cpp \
    data/wizard-page-create-database-configure.cpp \
    data/wizard-page-create-database-confirm.cpp \
    data/wizard-page-create-database-operation.cpp \
    data/i-dialog-insert-record.cpp

DATA_HEADERS += \
    data/data-navigator.h \
    data/date-query.h \
    data/numeric-query.h \
    data/text-query.h \
    data/wizard-backup-database.h \
    data/dialog-database-config.h \
    data/widget-edit-table.h \
    data/dialog-edit-table.h \
    data/dialog-insert-item.h \
    data/dialog-edit-item.h \
    data/wizard-create-database.h \
    data/wizard-page-create-database-intro.h \
    data/wizard-page-create-database-configure.h \
    data/wizard-page-create-database-confirm.h \
    data/wizard-page-create-database-operation.h \
    data/i-dialog-insert-record.h
EDITORS_HEADERS += \
    editors/text-editor-window.h \
    editors/editable-label.h \
    editors/sexagesimal-spinbox.h
MISC_HEADERS += \
    misc/choose-file.h \
    misc/color-button.hpp

SECURITY_HEADERS += \
    security/dialog-user-login.h

HEADERS = $$DATA_HEADERS $$EDITORS_HEADERS $$MISC_HEADERS $$SECURITY_HEADERS \
    qtz-widgets.h

FORMS += \
    data/data-navigator.ui \
    data/date-query.ui \
    data/numeric-query.ui \
    data/text-query.ui \
    data/wizard-backup-database.ui \
    data/dialog-database-config.ui \
    data/dialog-edit-table.ui \
    editors/text-editor-window.ui \
    editors/editable-label.ui \
    misc/choose-file.ui \
    security/dialog-user-login.ui \
    data/widget-edit-table.ui \
    data/wizard-page-create-database-intro.ui \
    data/wizard-page-create-database-configure.ui \
    data/wizard-page-create-database-confirm.ui \
    data/wizard-page-create-database-operation.ui

headers_data.files = $$DATA_HEADERS
headers_editors.files = $$EDITORS_HEADERS
headers_misc.files = $$MISC_HEADERS
headers_security.files = $$SECURITY_HEADERS

INSTALLS += target
INSTALLS += headers_data headers_editors headers_misc headers_security

RESOURCES += \
    QtzWidgets.qrc

OTHER_FILES += \
    data/schema-specified.xsd
