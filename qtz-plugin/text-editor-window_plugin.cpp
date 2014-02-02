#include <qtz/widgets/editors/text-editor-window.h>
#include "text-editor-window_plugin.h"

#include <QtCore/QtPlugin>

TextEditorWindowPlugin::TextEditorWindowPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.phemto.qtz.textEditorWindowPlugin")
#endif
}

void TextEditorWindowPlugin::initialize(QDesignerFormEditorInterface
                                        * /* core */)
{
    if (m_initialized) {
        return;
    }
    // Add extension registrations, etc. here
    m_initialized = true;
}

bool TextEditorWindowPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *TextEditorWindowPlugin::createWidget(QWidget *parent)
{
    return new TextEditorWindow(parent);
}

QString TextEditorWindowPlugin::name() const
{
    return QLatin1String("TextEditorWindow");
}

QString TextEditorWindowPlugin::group() const
{
    return QLatin1String("Qtz Editors");
}

QIcon TextEditorWindowPlugin::icon() const
{
    return QIcon(":/icons/images/qtzplaintextedit.png");
}

QString TextEditorWindowPlugin::toolTip() const
{
    return QLatin1String("This is a highly costumizable embedded text editor.");
}

QString TextEditorWindowPlugin::whatsThis() const
{
    return QLatin1String("Qtz embedded rich text editor.");
}

bool TextEditorWindowPlugin::isContainer() const
{
    return false;
}

QString TextEditorWindowPlugin::domXml() const
{
    return QLatin1String("<widget class=\"TextEditorWindow\" name=\"textEditor\">\n</widget>\n");
}

QString TextEditorWindowPlugin::includeFile() const
{
    return QLatin1String("qtz/widgets/editors/text-editor-window.h");
}

//#if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(qtz, TextEditorWindowPlugin)
//#endif
