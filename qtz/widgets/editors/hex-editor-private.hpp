#ifndef QTZ_HEXEDITPRIVATE_HPP
#define QTZ_HEXEDITPRIVATE_HPP

#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>
#include "hex-editor.hpp"
#include "hex-editor-document/hex-document.hpp"
#include "hex-editor-document/hex-theme.hpp"
#include "hex-editor-paint/hex-metrics.hpp"

class HexEditorPrivate : public QWidget {
    Q_OBJECT

public:
    explicit HexEditorPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget* parent = 0);
    HexDocument* document() const;
    HexMetrics* metrics() const;
    bool overwriteOnly() const;
    bool readOnly() const;
    void setDocument(HexDocument* document);
    void setOverwriteOnly(bool b);
    void setReadOnly(bool b);
    void scroll(QWheelEvent* event);

private:
    integer_t offsetFromPoint(const QPoint& pt, integer_t* bitindex = NULL) const;
    void toggleComment(const QPoint& pos);
    void updateCaret(integer_t offset, integer_t nibbleindex);
    void processDeleteEvents();
    void processBackspaceEvents();
    void processHexPart(int key);
    void processAsciiPart(int key);
    bool processMoveEvents(QKeyEvent* event);
    bool processSelectEvents(QKeyEvent* event);
    bool processTextInputEvents(QKeyEvent* event);
    bool processInsOvrEvents(QKeyEvent* event);
    bool processUndoRedo(QKeyEvent* event);
    bool processClipboardKeys(QKeyEvent* event);

protected:
    void paintEvent(QPaintEvent* pe);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void resizeEvent(QResizeEvent* e);

signals:
    void visibleLinesChanged();
    void verticalScroll(int value);

private:
    static const integer_t WHELL_SCROLL_LINES;
    QScrollArea* _scrollarea;
    QScrollBar* _vscrollbar;
    HexDocument* _document;
    HexMetrics* _metrics;
    HexTheme* _theme;
    bool _overwriteonly;
    bool _readonly;
};

#endif // QTZ_HEXEDITPRIVATE_HPP
