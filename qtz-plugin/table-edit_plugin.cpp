#include <qtz/widgets/data/widget-edit-table.h>
#include "table-edit_plugin.h"

#include <QtCore/QtPlugin>

TableEditPlugin::TableEditPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "org.fema.qtz.tableEditPlugin")
#endif
}

void TableEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool TableEditPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *TableEditPlugin::createWidget(QWidget *parent)
{
    return new WidgetEditTable(parent);
}

QString TableEditPlugin::name() const
{
    return QLatin1String("TableEdit");
}

QString TableEditPlugin::group() const
{
    return QLatin1String("Qtz SQL");
}

QIcon TableEditPlugin::icon() const
{
    return QIcon(":/icons/images/te.png");
}

QString TableEditPlugin::toolTip() const
{
    return QLatin1String("Edit items of a database table");
}

QString TableEditPlugin::whatsThis() const
{
    return QLatin1String("This widget provides basic operations on a database table.");
}

bool TableEditPlugin::isContainer() const
{
    return false;
}

QString TableEditPlugin::domXml() const
{
    return QLatin1String("<widget class=\"WidgetEditTable\" name=\"editTable\">\n</widget>\n");
}

QString TableEditPlugin::includeFile() const
{
    return QLatin1String("qtz/widgets/editors/table-edit_plugin.h");
}

