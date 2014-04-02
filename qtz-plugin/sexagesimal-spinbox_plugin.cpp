#include <qtz/widgets/editors/sexagesimal-spinbox.h>
#include "sexagesimal-spinbox_plugin.h"

#include <QtCore/QtPlugin>

SexagesimalSpinboxPlugin::SexagesimalSpinboxPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.phemto.qtz.sexagesimalSpinboxPlugin")
#endif
}

void SexagesimalSpinboxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool SexagesimalSpinboxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *SexagesimalSpinboxPlugin::createWidget(QWidget *parent)
{
    return new SexagesimalSpinBox(parent);
}

QString SexagesimalSpinboxPlugin::name() const
{
    return QLatin1String("SexagesimalSpinbox");
}

QString SexagesimalSpinboxPlugin::group() const
{
    return QLatin1String("Qtz Editors");
}

QIcon SexagesimalSpinboxPlugin::icon() const
{
    return QIcon(":/icons/images/ssb.png");
}

QString SexagesimalSpinboxPlugin::toolTip() const
{
    return QLatin1String("Sexagesimal Spin Box");
}

QString SexagesimalSpinboxPlugin::whatsThis() const
{
    return QLatin1String("This widget implements a spin box to edit and view sexagesimal values");
}

bool SexagesimalSpinboxPlugin::isContainer() const
{
    return false;
}

QString SexagesimalSpinboxPlugin::domXml() const
{
    return QLatin1String("<widget class=\"SexagesimalSpinbox\" name=\"sexagesimalSpinbox\">\n</widget>\n");
}

QString SexagesimalSpinboxPlugin::includeFile() const
{
    return QLatin1String("qtz/widgets/editors/sexagesimal-spinbox.h");
}

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(qtz, SexagesimalSpinboxPlugin)
//#endif
