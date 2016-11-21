#include "numeric-query_plugin.hpp"
#include "text-query_plugin.hpp"
#include "date-query_plugin.hpp"
#include "table-edit_plugin.h"
#include "text-editor-window_plugin.hpp"
#include "editable-label_plugin.hpp"
#include "sexagesimal-spinbox_plugin.hpp"
#include "choose-file_plugin.hpp"
#include "jalali-date-edit_plugin.hpp"
#include "qtz.hpp"

Qtz::Qtz(QObject* parent)
    : QObject(parent) {
    m_widgets.append(new NumericQueryPlugin(this));
    m_widgets.append(new TextQueryPlugin(this));
    m_widgets.append(new DateQueryPlugin(this));
    m_widgets.append(new TextEditorWindowPlugin(this));
    m_widgets.append(new EditableLabelPlugin(this));
    m_widgets.append(new SexagesimalSpinboxPlugin(this));
    m_widgets.append(new ChooseFilePlugin(this));
    m_widgets.append(new JalaliDateEditPlugin(this));
    //m_widgets.append(new TableEditPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> Qtz::customWidgets() const {
    return m_widgets;
}

#if QT_VERSION >= 0x050000
Q_PLUGIN_METADATA(Qtz)
#endif

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qtz, Qtz)
#endif
