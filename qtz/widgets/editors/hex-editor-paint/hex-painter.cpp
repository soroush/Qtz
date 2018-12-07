#include "hex-painter.hpp"
#include <QPaintEvent>
#include <QPainter>

#define containerWidget static_cast<QWidget*>(this->parent())

QString HexPainter::UNPRINTABLE_CHAR;

HexPainter::HexPainter(HexMetrics *metrics, QWidget *parent) : QObject(parent), _metrics(metrics), _document(metrics->document()), _vscrollbar(metrics->verticalScrollBar())
{
    if(HexPainter::UNPRINTABLE_CHAR.isEmpty())
        HexPainter::UNPRINTABLE_CHAR = ".";

    this->_boldfont = containerWidget->font();
    this->_boldfont.setBold(true);
}

void HexPainter::paint(QPaintEvent *e, HexTheme *theme)
{
    QPainter painter(containerWidget);

    this->drawLines(e, &painter, theme);
    this->drawBackground(&painter);
    this->drawCursor(&painter);
}

void HexPainter::drawCursor(QPainter *painter)
{
    if(!this->_document || !containerWidget->hasFocus())
        return;

    HexCursor* cursor = this->_document->cursor();

    if(!cursor->blinking())
        return;

    HexMetrics* metrics = this->_metrics;

    if(cursor->isInsertMode())
        painter->fillRect(cursor->cursorX(), cursor->cursorY(), 2, metrics->charHeight(), Qt::black);
    else
        painter->fillRect(cursor->cursorX(), cursor->cursorY() + metrics->charHeight() - 3, metrics->charWidth(), 2, Qt::black);
}

void HexPainter::drawBackground(QPainter* painter)
{
    integer_t span = this->_metrics->charWidth() / 2;

    painter->setBackgroundMode(Qt::TransparentMode);
    painter->setPen(containerWidget->palette().color(QPalette::WindowText));
    painter->drawLine(this->_metrics->xPosHex() - span, 0, this->_metrics->xPosHex() - span, containerWidget->height());
    painter->drawLine(this->_metrics->xPosAscii() - span, 0, this->_metrics->xPosAscii() - span, containerWidget->height());
    painter->drawLine(this->_metrics->xPosEnd() + span, 0, this->_metrics->xPosEnd() + span, containerWidget->height());
}

void HexPainter::drawLines(QPaintEvent *e, QPainter* painter, HexTheme *theme)
{
    if(!this->_document)
        return;

    QRect r = e->rect();
    integer_t slidepos = this->_vscrollbar->isVisible() ? this->_vscrollbar->sliderPosition() : 0;
    integer_t start = slidepos + (r.top() / this->_metrics->charHeight());
    integer_t end = (slidepos + (r.bottom() / this->_metrics->charHeight())) + 1; // end + 1 removes the scroll bug

    for(integer_t i = start; i <= end; i++)
    {
        int y = (i - slidepos) * this->_metrics->charHeight();
        this->drawLine(painter, theme, i, y);
    }
}

void HexPainter::drawLine(QPainter *painter, HexTheme* theme, integer_t line, integer_t y)
{
    integer_t xhex = this->_metrics->xPosHex(), xascii = this->_metrics->xPosAscii();
    integer_t linestart = line * HexMetrics::BYTES_PER_LINE;

    painter->setBackgroundMode(Qt::TransparentMode);
    painter->setFont(containerWidget->font());
    this->drawLineBackground(painter, theme, line, linestart, y);
    this->drawAddress(painter, theme, line, linestart, y);

    for(sinteger_t i = 0; i < HexMetrics::BYTES_PER_LINE; i++)
    {
        integer_t offset = linestart + i;

        if(offset >= this->_document->length())
            return; // Reached EOF

        uchar b = this->_document->at(offset);
        painter->setFont(containerWidget->font());

        this->colorize(painter, offset, b);
        this->drawHex(painter, b, i, offset, xhex, y);
        this->drawAscii(painter, b, offset, xascii, y);
    }
}

void HexPainter::drawLineBackground(QPainter *painter, HexTheme *theme, integer_t line, integer_t linestart, integer_t y)
{
    HexCursor* cursor = this->_document->cursor();
    QRect hexr(this->_metrics->xPosHex(), y, this->_metrics->xPosAscii() - (this->_metrics->charWidth() / 2), this->_metrics->charHeight());
    QRect asciir(this->_metrics->xPosAscii(), y, (this->_metrics->charWidth() * HexMetrics::BYTES_PER_LINE) + (this->_metrics->charWidth() / 2), this->_metrics->charHeight());

    if((cursor->offset() >= linestart) && (cursor->offset() < (linestart + HexMetrics::BYTES_PER_LINE))) // This is the Selected Line
    {
        painter->fillRect(hexr, theme->lineColor());
        painter->fillRect(asciir, theme->lineColor());
    }
    else if(line & 1)
    {
        painter->fillRect(hexr, theme->alternateLineColor());
        painter->fillRect(asciir, theme->alternateLineColor());
    }
    else
    {
        painter->fillRect(hexr, theme->baseColor());
        painter->fillRect(asciir, theme->baseColor());
    }
}

void HexPainter::drawAddress(QPainter *painter, HexTheme* theme, integer_t line, integer_t linestart, integer_t y)
{
    HexCursor* cursor = this->_document->cursor();
    QFontMetrics fm = containerWidget->fontMetrics();
    integer_t linemax = (this->_document->length() + HexMetrics::BYTES_PER_LINE - 1) / HexMetrics::BYTES_PER_LINE;
    painter->fillRect(0, y, this->_metrics->xPosHex() - (this->_metrics->charWidth() / 2), this->_metrics->charHeight(), theme->addressBackground());

    if(!this->_document || (line >= linemax))
        return;

    if((cursor->offset() >= linestart) && (cursor->offset() < (linestart + HexMetrics::BYTES_PER_LINE))) // This is the Selected Line
        painter->setPen(theme->alternateAddressForeground());
    else
        painter->setPen(theme->addressForeground());

    QString addr = QString("%1").arg(this->_document->baseAddress() + (line * HexMetrics::BYTES_PER_LINE), this->_metrics->addressWidth(), 16, QLatin1Char('0')).toUpper();
    painter->drawText(0, y, fm.width(addr), this->_metrics->charHeight(), Qt::AlignLeft | Qt::AlignTop, addr);
}

void HexPainter::drawHex(QPainter *painter, uchar b, sinteger_t i, integer_t offset, integer_t &x, integer_t y)
{
    QString s = QString("%1").arg(b, 2, 16, QLatin1Char('0')).toUpper();
    QFontMetrics fm = containerWidget->fontMetrics();
    QRect r(x, y, fm.width(s), this->_metrics->charHeight());

    this->mark(painter, r, offset, HexCursor::HexPart);

    if(i < (HexMetrics::BYTES_PER_LINE - 1))
        r.setWidth(r.width() + this->_metrics->charWidth());

    if(painter->backgroundMode() != Qt::TransparentMode)
        painter->fillRect(r, painter->background()); // NOTE: It's a bit ugly, but it works

    painter->drawText(r, Qt::AlignLeft | Qt::AlignTop, s);
    x += r.width();
}

void HexPainter::drawAscii(QPainter *painter, uchar b, integer_t offset, integer_t &x, integer_t y)
{
    QFontMetrics fm = containerWidget->fontMetrics();
    integer_t w;
    QString s;

    if(QChar(b).isPrint())
    {
        w = fm.width(b);
        s = QString(b);
    }
    else
    {
        w = fm.width(HexPainter::UNPRINTABLE_CHAR);
        s = HexPainter::UNPRINTABLE_CHAR;
    }

    QRect r(x, y, w, this->_metrics->charHeight());
    this->mark(painter, r, offset, HexCursor::AsciiPart);

    painter->drawText(r, Qt::AlignLeft | Qt::AlignTop, s);
    x += w;
}

void HexPainter::colorize(QPainter *painter, integer_t offset, uchar b)
{
    HexCursor* cursor = this->_document->cursor();

    if(cursor->isSelected(offset))
    {
        const QPalette& palette = containerWidget->palette();
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->setBackground(palette.highlight());
        painter->setPen(palette.highlightedText().color());
        return;
    }

    // Prepare default palette
    painter->setBackgroundMode(Qt::TransparentMode);
    painter->setPen(Qt::black);

    if(this->applyMetadata(painter, offset))
        return;

    if((b == 0x00) || (b == 0xFF))
        painter->setPen(Qt::darkGray);
}

bool HexPainter::applyMetadata(QPainter *painter, integer_t offset)
{
    HexMetadata* metadata = this->_document->metadata();
    MetadataList metalist = metadata->fromOffset(offset);
    bool applied = false;

    foreach(HexMetadataItem* metaitem, metalist)
    {
        if(!metaitem->contains(offset))
            continue;

        applied = true;

        if(metaitem->hasBackColor())
        {
            painter->setBackgroundMode(Qt::OpaqueMode);
            painter->setBackground(QBrush(metaitem->backColor()));
        }

        if(metaitem->hasForeColor())
            painter->setPen(metaitem->foreColor());

        if(metaitem->hasComment())
            painter->setFont(this->_boldfont);
    }

    return applied;
}

bool HexPainter::mark(QPainter *painter, const QRect &r, integer_t offset, HexCursor::SelectedPart part)
{
    HexCursor* cursor = this->_document->cursor();

    if((offset != cursor->offset()) || cursor->isAddressPartSelected() || (cursor->selectedPart() == part))
        return false;

    painter->fillRect(r, Qt::darkGray);
    painter->setPen(Qt::white);
    return true;
}
