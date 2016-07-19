#include <qtz/widgets/editors/sexagesimal-spinbox.h>
#include "sexagesimal-spinbox_plugin.h"

#include <QtCore/QtPlugin>

SexagesimalSpinBoxPlugin::SexagesimalSpinBoxPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.Ametis.qtz.sexagesimalSpinboxPlugin")
#endif
}

void SexagesimalSpinBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool SexagesimalSpinBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *SexagesimalSpinBoxPlugin::createWidget(QWidget *parent)
{
    return new SexagesimalSpinBox(parent);
}

QString SexagesimalSpinBoxPlugin::name() const
{
    return QLatin1String("SexagesimalSpinBox");
}

QString SexagesimalSpinBoxPlugin::group() const
{
    return QLatin1String("Qtz Editors");
}

QIcon SexagesimalSpinBoxPlugin::icon() const
{
    return QIcon(":/icons/images/ssb.png");
}

QString SexagesimalSpinBoxPlugin::toolTip() const
{
    return QLatin1String("Sexagesimal Spin Box");
}

QString SexagesimalSpinBoxPlugin::whatsThis() const
{
    return QLatin1String("This widget implements a spin box to edit and view sexagesimal values");
}

bool SexagesimalSpinBoxPlugin::isContainer() const
{
    return false;
}

QString SexagesimalSpinBoxPlugin::domXml() const
{
    return QLatin1String("<widget class=\"SexagesimalSpinBox\" name=\"sexagesimalSpinBox\">\n</widget>\n");
}

QString SexagesimalSpinBoxPlugin::includeFile() const
{
    return QLatin1String("qtz/widgets/editors/sexagesimal-spinbox.h");
}

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(qtz, SexagesimalSpinboxPlugin)
//#endif
