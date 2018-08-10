#include <qtz/widgets/misc/color-button.hpp>
#include "color-button_plugin.hpp"

#include <QtCore/QtPlugin>

ColorButtonPlugin::ColorButtonPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.colorButtonPlugin")
}

void ColorButtonPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool ColorButtonPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* ColorButtonPlugin::createWidget(QWidget* parent) {
    return new ColorButton(parent);
}

QString ColorButtonPlugin::name() const {
    return QLatin1String("ColorButton");
}

QString ColorButtonPlugin::group() const {
    return QLatin1String("Qtz Widgets");
}

QIcon ColorButtonPlugin::icon() const {
    return QIcon::fromTheme("color-management");
    // TODO: Add fallback icon
}

QString ColorButtonPlugin::toolTip() const {
    return QLatin1String("Color Pickup Button");
}

QString ColorButtonPlugin::whatsThis() const {
    return QLatin1String("This widget implements a button to open QColorDialog");
}

bool ColorButtonPlugin::isContainer() const {
    return false;
}

QString ColorButtonPlugin::domXml() const {
    return QLatin1String("<widget class=\"ColorButton\" name=\"colorPlugin\">\n</widget>\n");
}

QString ColorButtonPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/misc/color-button.hpp");
}
