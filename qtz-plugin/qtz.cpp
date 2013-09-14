#include "numeric-query_plugin.h"
#include "text-query_plugin.h"
#include "date-query_plugin.h"
#include "table-edit_plugin.h"
#include "text-editor-window_plugin.h"
#include "editable-label_plugin.h"
#include "choose-file_plugin.h"
#include "qtz.h"

Qtz::Qtz(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new NumericQueryPlugin(this));
    m_widgets.append(new TextQueryPlugin(this));
    m_widgets.append(new DateQueryPlugin(this));    
    m_widgets.append(new TextEditorWindowPlugin(this));
    m_widgets.append(new EditableLabelPlugin(this));
    m_widgets.append(new ChooseFilePlugin(this));
    m_widgets.append(new TableEditPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> Qtz::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION >= 0x050000
Q_PLUGIN_METADATA (Qtz)
#endif

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qtz, Qtz)
#endif
