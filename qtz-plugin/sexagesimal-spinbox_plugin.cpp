#include <qtz/widgets/editors/sexagesimal-spinbox.hpp>
#include "sexagesimal-spinbox_plugin.hpp"

#include <QtCore/QtPlugin>

SexagesimalSpinboxPlugin::SexagesimalSpinboxPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.sexagesimalSpinboxPlugin")
}

void SexagesimalSpinboxPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool SexagesimalSpinboxPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* SexagesimalSpinboxPlugin::createWidget(QWidget* parent) {
    return new SexagesimalSpinBox(parent);
}

QString SexagesimalSpinboxPlugin::name() const {
    return QLatin1String("SexagesimalSpinBox");
}

QString SexagesimalSpinboxPlugin::group() const {
    return QLatin1String("Qtz Editors");
}

QIcon SexagesimalSpinboxPlugin::icon() const {
    return QIcon::fromTheme("add-placemark",
                            QIcon{":/qtz_plugin/images/compass.png"});
}

QString SexagesimalSpinboxPlugin::toolTip() const {
    return QLatin1String("Sexagesimal Spin Box");
}

QString SexagesimalSpinboxPlugin::whatsThis() const {
    return QLatin1String("This widget implements a spin box to edit and view sexagesimal values");
}

bool SexagesimalSpinboxPlugin::isContainer() const {
    return false;
}

QString SexagesimalSpinboxPlugin::domXml() const {
    return QLatin1String("<widget class=\"SexagesimalSpinBox\" name=\"sexagesimalSpinBox\">\n</widget>\n");
}

QString SexagesimalSpinboxPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/editors/sexagesimal-spinbox.hpp");
}
