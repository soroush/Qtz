#include "data/numeric-query_plugin.h"
#include "data/text-query_plugin.h"
#include "data/date-query_plugin.h"
#include "editors/text-editor-window_plugin.h"
#include "editors/editable-label_plugin.h"
#include "misc/choose-file_plugin.h"
#include "agt.h"

AGT::AGT(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new NumericQueryPlugin(this));
    m_widgets.append(new TextQueryPlugin(this));
    m_widgets.append(new DateQueryPlugin(this));    
    m_widgets.append(new TextEditorWindowPlugin(this));
    m_widgets.append(new EditableLabelPlugin(this));
    m_widgets.append(new ChooseFilePlugin(this));
}

QList<QDesignerCustomWidgetInterface*> AGT::customWidgets() const
{
    return m_widgets;
}

Q_EXPORT_PLUGIN2(agtplugin, AGT)
