#include <agt/ui/editors/editable-label.h>
#include "editable-label_plugin.h"

#include <QtCore/QtPlugin>

EditableLabelPlugin::EditableLabelPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
    Q_PLUGIN_METADATA(IID "org.fema.agt.editableLabelPlugin")
}

void EditableLabelPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool EditableLabelPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *EditableLabelPlugin::createWidget(QWidget *parent)
{
    return new EditableLabel(parent);
}

QString EditableLabelPlugin::name() const
{
    return QLatin1String("EditableLabel");
}

QString EditableLabelPlugin::group() const
{
    return QLatin1String("AGT Editors");
}

QIcon EditableLabelPlugin::icon() const
{
    return QIcon(":/icons/images/dq.png");
}

QString EditableLabelPlugin::toolTip() const
{
    return QLatin1String("A Label class that can be edited by AGT advanced editor in runtime");
}

QString EditableLabelPlugin::whatsThis() const
{
    return QLatin1String("This widget...");
}

bool EditableLabelPlugin::isContainer() const
{
    return false;
}

QString EditableLabelPlugin::domXml() const
{
    return QLatin1String("<widget class=\"EditableLabel\" name=\"editableLabel\">\n</widget>\n");
}

QString EditableLabelPlugin::includeFile() const
{
    return QLatin1String("agt/ui/editors/editable-label.h");
}

