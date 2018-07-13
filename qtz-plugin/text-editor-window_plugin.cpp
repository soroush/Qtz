#include <qtz/widgets/editors/text-editor-window.hpp>
#include "text-editor-window_plugin.hpp"

#include <QtCore/QtPlugin>

TextEditorWindowPlugin::TextEditorWindowPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.textEditorWindowPlugin")
}

void TextEditorWindowPlugin::initialize(QDesignerFormEditorInterface
                                        * /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool TextEditorWindowPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* TextEditorWindowPlugin::createWidget(QWidget* parent) {
    return new TextEditorWindow(parent);
}

QString TextEditorWindowPlugin::name() const {
    return QLatin1String("TextEditorWindow");
}

QString TextEditorWindowPlugin::group() const {
    return QLatin1String("Qtz Editors");
}

QIcon TextEditorWindowPlugin::icon() const {
    return QIcon::fromTheme("story-editor",
                            QIcon{":/qtz_plugin/images/edit.png"});
}

QString TextEditorWindowPlugin::toolTip() const {
    return QLatin1String("This is a highly costumizable embedded text editor.");
}

QString TextEditorWindowPlugin::whatsThis() const {
    return QLatin1String("Qtz embedded rich text editor.");
}

bool TextEditorWindowPlugin::isContainer() const {
    return false;
}

QString TextEditorWindowPlugin::domXml() const {
    return QLatin1String("<widget class=\"TextEditorWindow\" name=\"textEditor\">\n</widget>\n");
}

QString TextEditorWindowPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/editors/text-editor-window.hpp");
}

