#include <agt/ui/data/date-query.h>
#include "date-query_plugin.h"

#include <QtCore/QtPlugin>

DateQueryPlugin::DateQueryPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void DateQueryPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool DateQueryPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *DateQueryPlugin::createWidget(QWidget *parent)
{
    return new DateQuery(parent);
}

QString DateQueryPlugin::name() const
{
    return QLatin1String("DateQuery");
}

QString DateQueryPlugin::group() const
{
    return QLatin1String("AGT SQL");
}

QIcon DateQueryPlugin::icon() const
{
    return QIcon(":/icons/images/dq.png");
}

QString DateQueryPlugin::toolTip() const
{
    return QLatin1String("Date and time value 'where' clause manipulator");
}

QString DateQueryPlugin::whatsThis() const
{
    return QLatin1String("This widget manipulates SQL queries for date and time values.");
}

bool DateQueryPlugin::isContainer() const
{
    return false;
}

QString DateQueryPlugin::domXml() const
{
    return QLatin1String("<widget class=\"DateQuery\" name=\"dateQuery\">\n</widget>\n");
}

QString DateQueryPlugin::includeFile() const
{
    return QLatin1String("agt/ui/date-query.h");
}

