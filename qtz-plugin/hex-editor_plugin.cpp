#include <qtz/widgets/editors/hex-editor.hpp>
#include "hex-editor_plugin.hpp"

#include <QtCore/QtPlugin>

HexEditorPlugin::HexEditorPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.hexEditorPlugin")
}

void HexEditorPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool HexEditorPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* HexEditorPlugin::createWidget(QWidget* parent) {
    return new HexEditor(parent);
}

QString HexEditorPlugin::name() const {
    return QLatin1String("HexEditor");
}

QString HexEditorPlugin::group() const {
    return QLatin1String("Qtz Widgets");
}

QIcon HexEditorPlugin::icon() const {
    return QIcon::fromTheme("texture",
                            QIcon{":/qtz_plugin/images/hex-edit.png"});
}

QString HexEditorPlugin::toolTip() const {
    return QLatin1String("Hexadecimal Editor Widget");
}

QString HexEditorPlugin::whatsThis() const {
    return QLatin1String("This is a hexadecimal editor widget.");
}

bool HexEditorPlugin::isContainer() const {
    return false;
}

QString HexEditorPlugin::domXml() const {
    return QLatin1String("<widget class=\"HexEditor\" name=\"hexEditor\">\n</widget>\n");
}

QString HexEditorPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/editors/hex-editor.hpp");
}
