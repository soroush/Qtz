#ifndef QTZ_WIDGETS_HEXEDITOR_HPP
#define QTZ_WIDGETS_HEXEDITOR_HPP

#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include "hex-editor-document/hex-document.hpp"
#include "hex-editor-paint/hex-metrics.hpp"
#include "../qtz-widgets.hpp"

class HexEditorPrivate;

class QTZ_WIDGETS_SHARED_EXPORT HexEditor : public QFrame {
    Q_OBJECT

public:
    explicit HexEditor(QWidget* parent = 0);
    HexDocument* document() const;
    HexMetrics* metrics() const;
    bool overwriteOnly() const;
    bool readOnly() const;

public slots:
    void setOverwriteOnly(bool b);
    void setReadOnly(bool b);
    void setDocument(HexDocument* document);
    void scroll(QWheelEvent* event);

signals:
    void verticalScroll(integer_t value);
    void visibleLinesChanged();

private:
    HexEditorPrivate* _hexedit_p;
    QScrollArea* _scrollarea;
    QScrollBar* _vscrollbar;
    QHBoxLayout* _hlayout;
};

#endif // QTZ_WIDGETS_HEXEDITOR_HPP
