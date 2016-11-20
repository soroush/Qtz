#include <qtz/widgets/data/numeric-query.hpp>
#include "numeric-query_plugin.hpp"

#include <QtCore/QtPlugin>

NumericQueryPlugin::NumericQueryPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    #if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.numericQueryPlugin")
    #endif
}

void NumericQueryPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool NumericQueryPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* NumericQueryPlugin::createWidget(QWidget* parent) {
    return new NumericQuery(parent);
}

QString NumericQueryPlugin::name() const {
    return QLatin1String("NumericQuery");
}

QString NumericQueryPlugin::group() const {
    return QLatin1String("Qtz SQL");
}

QIcon NumericQueryPlugin::icon() const {
    return QIcon(":/icons/images/nq.png");
}

QString NumericQueryPlugin::toolTip() const {
    return QLatin1String("Numeric value 'where' clause manipulator");
}

QString NumericQueryPlugin::whatsThis() const {
    return QLatin1String("This widget manipulates SQL queries for numeric values.");
}

bool NumericQueryPlugin::isContainer() const {
    return false;
}

QString NumericQueryPlugin::domXml() const {
    return QLatin1String("<widget class=\"NumericQuery\" name=\"numericQuery\">\n</widget>\n");
}

QString NumericQueryPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/data/numeric-query.h");
}

//#if QT_VERSION <= 0x050000
//Q_EXPORT_PLUGIN2(qtz, NumericQueryPlugin)
//#endif
