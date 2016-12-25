QT += core gui sql xml
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets designer
}
lessThan(QT_MAJOR_VERSION, 5){
    QMAKE_CXXFLAGS += -std=c++11
}

DEFINES += QTZ_WIDGETS_LIBRARY

TEMPLATE = lib
VERSION = 0.2.0

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
    headers.path = /usr/include/qtz/widgets
    headers_base.path = /usr/include/qtz/widgets
    headers_data.path = /usr/include/qtz/widgets/data
    headers_editors.path = /usr/include/qtz/widgets/editors
    headers_misc.path = /usr/include/qtz/widgets/misc
    headers_security.path = /usr/include/qtz/widgets/security
    headers_i18n.path = /usr/include/qtz/widgets/i18n
    headers_viewers.path = /usr/include/qtz/widgets/viewer
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
    headers.path = $$INSTALL_ROOT/qtz/widgets
    headers_base.path = $$INSTALL_ROOT/qtz/widgets
    headers.path = $$INSTALL_ROOT/include/qtz/widgets
    headers_data.path = $$INSTALL_ROOT/include/qtz/widgets/data
    headers_editors.path = $$INSTALL_ROOT/include/qtz/widgets/editors
    headers_misc.path = $$INSTALL_ROOT/include/qtz/widgets/misc
    headers_security.path = $$INSTALL_ROOT/include/qtz/widgets/security
    headers_i18n.path = $$INSTALL_ROOT/include/qtz/widgets/i18n
    headers_viewers.path = $$INSTALL_ROOT/include/qtz/widgets/viewer
    LINK_MAJ = "0"
    RC_FILE = QtzWidgets.rc
}

CONFIG(local){
    INCLUDEPATH += ../../
    lessThan(QT_MAJOR_VERSION, 5):{
        QMAKE_LIBDIR += "$$OUT_PWD/../core/$$BUILD"
        QMAKE_LIBDIR += "$$OUT_PWD/../data/$$BUILD"
        QMAKE_LIBDIR += "$$OUT_PWD/../security/$$BUILD"
    }
    greaterThan(QT_MAJOR_VERSION, 4): {
        QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../core/$$BUILD")
        QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../data/$$BUILD")
        QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../security/$$BUILD")
    }
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzSecurity$${BUILD_SUFFIX}$${LINK_MAJ}
}else {
    LIBS += -lQtzData$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzCore$${BUILD_SUFFIX}$${LINK_MAJ}
    LIBS += -lQtzSecurity$${BUILD_SUFFIX}$${LINK_MAJ}
}

# Add OpenCV dependency
CONFIG( debug, debug|release ) {
    LIBS+= -lopencv_world310d
} else {
    LIBS+= -lopencv_world310
}

SOURCES += \
    i18n/localizer.cpp \
    data/date-query.cpp \
    data/numeric-query.cpp \
    data/text-query.cpp \
    data/dialog-database-config.cpp \
    editors/text-editor-window.cpp \
    editors/editable-label.cpp \
    editors/sexagesimal-spinbox.cpp \
    editors/jalali-date-edit.cpp \
    viewers/image-viewer.cpp \
    misc/choose-file.cpp \
    misc/color-button.cpp \
    misc/font-button.cpp \
    data/i-dialog-insert-record.cpp \
    data/asynchronous-main-window.cpp

#    data/wizard-backup-database.cpp \
#    security/dialog-user-login.cpp \
#    data/widget-edit-table.cpp \
#    data/dialog-insert-item.cpp \
#    data/dialog-edit-item.cpp \
#    data/dialog-edit-table.cpp \
#    data/wizard-create-database.cpp \
#    data/wizard-page-create-database-intro.cpp \
#    data/wizard-page-create-database-configure.cpp \
#    data/wizard-page-create-database-confirm.cpp \
#    data/wizard-page-create-database-operation.cpp \
#    data/wizard-backup-database.hpp \
#    data/widget-edit-table.hpp \
#    data/dialog-edit-table.hpp \
#    data/dialog-insert-item.hpp \
#    data/dialog-edit-item.hpp \
#    data/wizard-create-database.hpp \
#    data/wizard-page-create-database-intro.hpp \
#    data/wizard-page-create-database-configure.hpp \
#    data/wizard-page-create-database-confirm.hpp \
#    data/wizard-page-create-database-operation.hpp \

HEADERS_DATA += \
    data/date-query.hpp \
    data/numeric-query.hpp \
    data/text-query.hpp \
    data/dialog-database-config.hpp \
    data/asynchronous-main-window.hpp \
    data/i-dialog-insert-record.hpp
HEADERS_EDITORS += \
    editors/text-editor-window.hpp \
    editors/editable-label.hpp \
    editors/jalali-date-edit.hpp \
    editors/sexagesimal-spinbox.hpp
HEADERS_MISC = \
    misc/choose-file.hpp \
    misc/color-button.hpp \
    misc/font-button.hpp
HEADERS_SECURITY = \
    security/dialog-user-login.hpp
HEADERS_I18N = \
    i18n/localizer.hpp
HEADERS_VIEWERS = \
    viewers/image-viewer.hpp

HEADERS_BASE = qtz-widgets.hpp

HEADERS += $$HEADERS_BASE \
    $$HEADERS_DATA \
    $$HEADERS_EDITORS \
    $$HEADERS_MISC \
    $$HEADERS_SECURITY \
    $$HEADERS_I18N \
    $$HEADERS_VIEWERS

FORMS += \
    data/date-query.ui \
    data/numeric-query.ui \
    data/text-query.ui \
    data/dialog-database-config.ui \
    data/dialog-edit-table.ui \
    editors/text-editor-window.ui \
    editors/editable-label.ui \
    misc/choose-file.ui \
    security/dialog-user-login.ui

#    data/wizard-backup-database.ui \
#    data/widget-edit-table.ui \
#    data/wizard-page-create-database-intro.ui \
#    data/wizard-page-create-database-configure.ui \
#    data/wizard-page-create-database-confirm.ui \
#    data/wizard-page-create-database-operation.ui


headers_data.files = $$HEADERS_DATA
headers_editors.files = $$HEADERS_EDITORS
headers_misc.files = $$HEADERS_MISC
headers_security.files = $$HEADERS_SECURITY
headers_i18n.files = $$HEADERS_I18N
headers_base.files = $$HEADERS_BASE
headers_base.viewers = $$HEADERS_VIEWERS

INSTALLS += target
INSTALLS += headers_base \
    headers_data \
    headers_editors \
    headers_misc \
    headers_security \
    headers_i18n \
    headers_viewers

RESOURCES += \
    QtzWidgets.qrc

OTHER_FILES += \
    data/schema-specified.xsd
