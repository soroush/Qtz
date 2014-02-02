#include <qtz/widgets/editors/editable-label.h>
#include "editable-label_plugin.h"

#include <QtCore/QtPlugin>

EditableLabelPlugin::EditableLabelPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.phemto.qtz.editableLabelPlugin")
#endif
}

void EditableLabelPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized) {
        return;
    }
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
    return QLatin1String("Qtz Editors");
}

QIcon EditableLabelPlugin::icon() const
{
    return QIcon(":/icons/images/dq.png");
}

QString EditableLabelPlugin::toolTip() const
{
    return QLatin1String("A Label class that can be edited by Qtz advanced editor in runtime");
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
    return QLatin1String("qtz/widgets/editors/editable-label.h");
}

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(qtz, EditableLabelPlugin)
//#endif
