#include <agt/ui/data/numeric-query.h>
#include "numeric-query_plugin.h"

#include <QtCore/QtPlugin>

NumericQueryPlugin::NumericQueryPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void NumericQueryPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool NumericQueryPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *NumericQueryPlugin::createWidget(QWidget *parent)
{
    return new NumericQuery(parent);
}

QString NumericQueryPlugin::name() const
{
    return QLatin1String("NumericQuery");
}

QString NumericQueryPlugin::group() const
{
    return QLatin1String("AGT SQL");
}

QIcon NumericQueryPlugin::icon() const
{
    return QIcon(":/icons/images/nq.png");
}

QString NumericQueryPlugin::toolTip() const
{
    return QLatin1String("Numeric value 'where' clause manipulator");
}

QString NumericQueryPlugin::whatsThis() const
{
    return QLatin1String("This widget manipulates SQL queries for numeric values.");
}

bool NumericQueryPlugin::isContainer() const
{
    return false;
}

QString NumericQueryPlugin::domXml() const
{
    return QLatin1String("<widget class=\"NumericQuery\" name=\"numericQuery\">\n</widget>\n");
}

QString NumericQueryPlugin::includeFile() const
{
    return QLatin1String("agt/ui/numeric-query.h");
}

