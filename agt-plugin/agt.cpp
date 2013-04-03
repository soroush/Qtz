#include "numeric-query_plugin.h"
#include "text-query_plugin.h"
#include "date-query_plugin.h"
#include "table-edit_plugin.h"
#include "text-editor-window_plugin.h"
#include "editable-label_plugin.h"
#include "choose-file_plugin.h"
#include "agt.h"

AGT::AGT(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new NumericQueryPlugin(this));
    m_widgets.append(new TextQueryPlugin(this));
    m_widgets.append(new DateQueryPlugin(this));    
    m_widgets.append(new TextEditorWindowPlugin(this));
    m_widgets.append(new EditableLabelPlugin(this));
//    m_widgets.append(new ChooseFilePlugin(this));
//    m_widgets.append(new TableEditPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> AGT::customWidgets() const
{
    return m_widgets;
}

Q_EXPORT_PLUGIN2(agtplugin, AGT)
