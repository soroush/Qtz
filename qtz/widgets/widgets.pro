QT += core gui sql xml network
CONFIG += c++11
QT += widgets

DEFINES += QTZ_WIDGETS_LIBRARY

TEMPLATE = lib
VERSION = 0.4.0

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
    headers_editors_hexdocument.path = /usr/include/qtz/widgets/editors/hex-editor-document
    headers_editors_hexdocument_commands.path = /usr/include/qtz/widgets/editors/hex-editor-document/commands
    headers_editors_hexdocument_metadata.path = /usr/include/qtz/widgets/editors/hex-editor-document/metadata
    headers_editors_hexpaint.path = /usr/include/qtz/widgets/editors/hex-editor-paint
    headers_misc.path = /usr/include/qtz/widgets/misc
    headers_security.path = /usr/include/qtz/widgets/security
    headers_i18n.path = /usr/include/qtz/widgets/i18n
    headers_viewers.path = /usr/include/qtz/widgets/viewers
    headers_application.path = /usr/include/qtz/widgets/application
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
    headers_editors_hexdocument.path = $$INSTALL_ROOT/include/qtz/widgets/editors/hex-editor-document
    headers_editors_hexdocument_commands.path = $$INSTALL_ROOT/include/qtz/widgets/editors/hex-editor-document/commands
    headers_editors_hexdocument_metadata.path = $$INSTALL_ROOT/include/qtz/widgets/editors/hex-editor-document/metadata
    headers_editors_hexpaint.path = $$INSTALL_ROOT/include/qtz/widgets/editors/hex-editor-paint
    headers_misc.path = $$INSTALL_ROOT/include/qtz/widgets/misc
    headers_security.path = $$INSTALL_ROOT/include/qtz/widgets/security
    headers_i18n.path = $$INSTALL_ROOT/include/qtz/widgets/i18n
    headers_viewers.path = $$INSTALL_ROOT/include/qtz/widgets/viewers
    headers_application.path = $$INSTALL_ROOT/include/qtz/widgets/application
    LINK_MAJ = "0"
    RC_FILE = QtzWidgets.rc
    msvc:LIBS += Advapi32.lib
    gcc:LIBS += -lAdvapi32
}

CONFIG(local){
    INCLUDEPATH += ../../
    QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../core/$$BUILD")
    QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../data/$$BUILD")
    QMAKE_LIBDIR += $$absolute_path("$$OUT_PWD/../security/$$BUILD")
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
    win32 {
        LIBS+= -lopencv_core340d -lopencv_imgproc340d -lopencv_imgcodecs340d
    }
} else {
    win32 {
        LIBS+= -lopencv_core340  -lopencv_imgproc340  -lopencv_imgcodecs340
    }
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

SOURCES += \
    application/qtz-single-application.cpp \
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
    data/asynchronous-main-window.cpp \
    editors/hex-editor-private.cpp \
    editors/hex-editor.cpp \
    editors/hex-editor-document/gap-buffer.cpp \
    editors/hex-editor-document/hex-cursor.cpp \
    editors/hex-editor-document/hex-document.cpp \
    editors/hex-editor-document/hex-theme.cpp \
    editors/hex-editor-document/commands/hex-command.cpp \
    editors/hex-editor-document/commands/insert-command.cpp \
    editors/hex-editor-document/commands/remove-command.cpp \
    editors/hex-editor-document/commands/replace-command.cpp \
    editors/hex-editor-document/metadata/hex-metadata.cpp \
    editors/hex-editor-document/metadata/hex-metadata-item.cpp \
    editors/hex-editor-paint/hex-metrics.cpp \
    editors/hex-editor-paint/hex-painter.cpp


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
    editors/sexagesimal-spinbox.hpp \
    editors/hex-editor.hpp \
    editors/hex-editor-private.hpp
HEADERS_EDITORS_HEXDOC += \
    editors/hex-editor-document/gap-buffer.hpp \
    editors/hex-editor-document/hex-cursor.hpp \
    editors/hex-editor-document/hex-document.hpp \
    editors/hex-editor-document/hex-theme.hpp
HEADERS_EDITORS_HEXDOC_COMMANDS += \
    editors/hex-editor-document/commands/hex-command.hpp \
    editors/hex-editor-document/commands/insert-command.hpp \
    editors/hex-editor-document/commands/remove-command.hpp \
    editors/hex-editor-document/commands/replace-command.hpp
HEADERS_EDITORS_HEXDOC_METADATA += \
    editors/hex-editor-document/metadata/hex-metadata-item.hpp \
    editors/hex-editor-document/metadata/hex-metadata.hpp
HEADERS_EDITORS_HEXDOC_PAINT += \
    editors/hex-editor-paint/hex-metrics.hpp \
    editors/hex-editor-paint/hex-painter.hpp
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
HEADERS_APPLICATION = \
    application/qtz-single-application.hpp \
    application/qtz-single-application_p.hpp
HEADERS_BASE = qtz-widgets.hpp

HEADERS += $$HEADERS_BASE \
    $$HEADERS_DATA \
    $$HEADERS_EDITORS \
    $$HEADERS_EDITORS_HEXDOC \
    $$HEADERS_EDITORS_HEXDOC_COMMANDS \
    $$HEADERS_EDITORS_HEXDOC_METADATA \
    $$HEADERS_EDITORS_HEXDOC_PAINT \
    $$HEADERS_MISC \
    $$HEADERS_SECURITY \
    $$HEADERS_I18N \
    $$HEADERS_VIEWERS \
    $$HEADERS_APPLICATION

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
headers_editors_hexdocument.files = $$HEADERS_EDITORS_HEXDOC
headers_editors_hexdocument_commands.files = $$HEADERS_EDITORS_HEXDOC_COMMANDS
headers_editors_hexdocument_metadata.files = $$HEADERS_EDITORS_HEXDOC_METADATA
headers_editors_hexpaint.files = $$HEADERS_EDITORS_HEXDOC_PAINT
headers_misc.files = $$HEADERS_MISC
headers_security.files = $$HEADERS_SECURITY
headers_i18n.files = $$HEADERS_I18N
headers_base.files = $$HEADERS_BASE
headers_viewers.files = $$HEADERS_VIEWERS
headers_application.files = $$HEADERS_APPLICATION

INSTALLS += target
INSTALLS += headers_base \
    headers_data \
    headers_editors \
    headers_editors_hexdocument \
    headers_editors_hexdocument_commands \
    headers_editors_hexdocument_metadata \
    headers_editors_hexpaint \
    headers_misc \
    headers_security \
    headers_i18n \
    headers_viewers \
    headers_application

OTHER_FILES += \
    data/schema-specified.xsd
