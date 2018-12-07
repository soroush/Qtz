#ifndef QTZ_WIDGETS_HEXPAINTER_HPP
#define QTZ_WIDGETS_HEXPAINTER_HPP

#include <QScrollBar>
#include "../hex-editor-document/hex-document.hpp"
#include "../hex-editor-document/hex-theme.hpp"
#include "hex-metrics.hpp"

class HexPainter : public QObject {
    Q_OBJECT

public:
    explicit HexPainter(HexMetrics* metrics, QWidget* parent = 0);
    ~HexPainter() = default;
    void paint(QPaintEvent* e, HexTheme* theme);

private:
    void drawCursor(QPainter* painter);
    void drawBackground(QPainter* painter);
    void drawLines(QPaintEvent* e, QPainter* painter, HexTheme* theme);
    void drawLine(QPainter* painter, HexTheme* theme, integer_t line, integer_t y);
    void drawLineBackground(QPainter* painter, HexTheme* theme, integer_t line, integer_t linestart, integer_t y);
    void drawAddress(QPainter* painter, HexTheme* theme, integer_t line, integer_t linestart, integer_t y);
    void drawHex(QPainter* painter, uchar b, sinteger_t i, integer_t offset, integer_t& x, integer_t y);
    void drawAscii(QPainter* painter, uchar b, integer_t offset, integer_t& x, integer_t y);
    void colorize(QPainter* painter, integer_t offset, uchar b);
    bool applyMetadata(QPainter* painter, integer_t offset);
    bool mark(QPainter* painter, const QRect& r, integer_t offset, HexCursor::SelectedPart part);

private:
    static QString UNPRINTABLE_CHAR;
    HexMetrics* _metrics;
    HexDocument* _document;
    QScrollBar* _vscrollbar;
    QFont _boldfont;
};

#endif // QTZ_WIDGETS_HEXPAINTER_HPP
