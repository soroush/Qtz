#include <qtz/widgets/data/text-query.hpp>
#include "text-query_plugin.hpp"

#include <QtCore/QtPlugin>

TextQueryPlugin::TextQueryPlugin(QObject* parent)
    : QObject(parent) {
    m_initialized = false;
    #if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.ametis.qtz.textQueryPlugin")
    #endif
}

void TextQueryPlugin::initialize(QDesignerFormEditorInterface* /* core */) {
    if(m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool TextQueryPlugin::isInitialized() const {
    return m_initialized;
}

QWidget* TextQueryPlugin::createWidget(QWidget* parent) {
    return new TextQuery(parent);
}

QString TextQueryPlugin::name() const {
    return QLatin1String("TextQuery");
}

QString TextQueryPlugin::group() const {
    return QLatin1String("Qtz SQL");
}

QIcon TextQueryPlugin::icon() const {
    return QIcon(":/qtz_plugin/images/text-query.png");
}

QString TextQueryPlugin::toolTip() const {
    return QLatin1String("String value 'where' clause manipulator");
}

QString TextQueryPlugin::whatsThis() const {
    return QLatin1String("This widget manipulates SQL queries for string values.");
}

bool TextQueryPlugin::isContainer() const {
    return false;
}

QString TextQueryPlugin::domXml() const {
    return QLatin1String("<widget class=\"TextQuery\" name=\"textQuery\">\n</widget>\n");
}

QString TextQueryPlugin::includeFile() const {
    return QLatin1String("qtz/widgets/data/text-query.hpp");
}

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(qtz, TextQueryPlugin)
//#endif
