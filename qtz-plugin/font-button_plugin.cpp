#include <qtz/widgets/misc/font-button.hpp>
#include "font-button_plugin.hpp"

#include <QtCore/QtPlugin>

FontButtonPlugin::FontButtonPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.fontButtonPlugin")
}

void FontButtonPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool FontButtonPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* FontButtonPlugin::createWidget(QWidget* parent) {
    return new FontButton(parent);
}

QString FontButtonPlugin::name() const {
    return QLatin1String("FontButton");
}

QString FontButtonPlugin::group() const {
    return QLatin1String("Qtz Widgets");
}

QIcon FontButtonPlugin::icon() const {
    return QIcon::fromTheme("gtk-select-font");
    // TODO: add fallback icon
}

QString FontButtonPlugin::toolTip() const {
    return QLatin1String("Font Select Button");
}

QString FontButtonPlugin::whatsThis() const {
    return QLatin1String("This widget implements a button to open font select dialog");
}

bool FontButtonPlugin::isContainer() const {
    return false;
}

QString FontButtonPlugin::domXml() const {
    return QLatin1String("<widget class=\"FontButton\" name=\"fontButton\">\n</widget>\n");
}

QString FontButtonPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/misc/font-button.hpp");
}
