#include <qtz/widgets/misc/choose-file.h>
#include "choose-file_plugin.h"

#include <QtCore/QtPlugin>

ChooseFilePlugin::ChooseFilePlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.Ametis.qtz.chooseFilePlugin")
#endif
}

void ChooseFilePlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool ChooseFilePlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ChooseFilePlugin::createWidget(QWidget *parent)
{
    return new ChooseFile(parent);
}

QString ChooseFilePlugin::name() const
{
    return QLatin1String("ChooseFile");
}

QString ChooseFilePlugin::group() const
{
    return QLatin1String("Qtz Widgets");
}

QIcon ChooseFilePlugin::icon() const
{
    return QIcon(":/icons/images/dq.png");
}

QString ChooseFilePlugin::toolTip() const
{
    return QLatin1String("Choose file widget");
}

QString ChooseFilePlugin::whatsThis() const
{
    return QLatin1String("This widget selects a file by eitehr its path or a file dilalog.");
}

bool ChooseFilePlugin::isContainer() const
{
    return false;
}

QString ChooseFilePlugin::domXml() const
{
    return QLatin1String("<widget class=\"ChooseFile\" name=\"chooseFile\">\n</widget>\n");
}

QString ChooseFilePlugin::includeFile() const
{
    return QLatin1String("qtz/widgets/misc/choose-file.h");
}

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(qtz, ChooseFilePlugin)
//#endif
