#ifndef QTZ_WIDGETS_HEXMETRICS_HPP
#define QTZ_WIDGETS_HEXMETRICS_HPP

#include <QScrollBar>
#include <QFontMetrics>
#include <QFont>
#include "../hex-editor-document/hex-document.hpp"

class HexMetrics : public QObject {
    Q_OBJECT

public:
    explicit HexMetrics(QScrollBar* vscrollbar, QWidget* parent = 0);
    ~HexMetrics() = default;
    HexDocument* document() const;
    QScrollBar* verticalScrollBar() const;
    QSize charSize() const;
    integer_t visibleStartOffset() const;
    integer_t visibleEndOffset() const;
    integer_t visibleLines() const;
    integer_t currentLine() const;
    integer_t addressWidth() const;
    integer_t charWidth() const;
    integer_t charHeight() const;
    sinteger_t xPosAscii() const;
    sinteger_t xPosHex() const;
    sinteger_t xPosEnd() const;
    void calculate(HexDocument* document, const QFontMetrics& fm);
    void calculate(const QFontMetrics& fm);
    bool ensureVisible();

private:
    void calculateAddressWidth();

public:
    static const sinteger_t BYTES_PER_LINE;

private:
    static const sinteger_t DEFAULT_ADDRESS_WIDTH;
    QScrollBar* _vscrollbar;
    HexDocument* _document;
    sinteger_t _addresswidth;
    sinteger_t _charwidth, _charheight;
    sinteger_t _xposascii, _xposhex, _xposend;
};

#endif // QTZ_WIDGETS_HEXMETRICS_HPP
