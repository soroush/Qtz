#include "hex-editor-private.hpp"
#include "hex-editor-paint/hex-painter.hpp"
#include <QKeyEvent>
#include <QToolTip>

const integer_t HexEditorPrivate::WHELL_SCROLL_LINES = 5;

HexEditorPrivate::HexEditorPrivate(QScrollArea* scrollarea, QScrollBar* vscrollbar, QWidget* parent): QWidget(parent),
    _document(NULL) {
    this->_theme = new HexTheme(this);
    this->_theme->setBaseColor(this->palette().color(QPalette::Base));

    this->_metrics = new HexMetrics(vscrollbar, this);

    this->_scrollarea = scrollarea;
    this->_vscrollbar = vscrollbar;
    this->_overwriteonly = false;
    this->_readonly = false;

    connect(this->_vscrollbar, &QScrollBar::valueChanged, [this] { this->update(); });
    connect(this->_vscrollbar, &QScrollBar::valueChanged, this, &HexEditorPrivate::verticalScroll);

    this->setMouseTracking(true);
    this->setCursor(Qt::IBeamCursor);
    this->setFocusPolicy(Qt::StrongFocus);

    this->_metrics->calculate(this->fontMetrics());
}

HexDocument* HexEditorPrivate::document() const {
    return this->_document;
}

HexMetrics* HexEditorPrivate::metrics() const {
    return this->_metrics;
}

bool HexEditorPrivate::overwriteOnly() const {
    return this->_overwriteonly;
}

bool HexEditorPrivate::readOnly() const {
    return this->_readonly;
}

void HexEditorPrivate::setDocument(HexDocument* document) {
    if(this->_document) {
        disconnect(this->_document, &HexDocument::canUndoChanged, this, 0);
        disconnect(this->_document, &HexDocument::canRedoChanged, this, 0);
        disconnect(this->_document, &HexDocument::documentChanged, this, 0);
        disconnect(this->_document, &HexDocument::baseAddressChanged, this, 0);
        disconnect(this->_document->cursor(), &HexCursor::blinkChanged, this, 0);
        disconnect(this->_document->cursor(), &HexCursor::positionChanged, this, 0);
        disconnect(this->_document->cursor(), &HexCursor::offsetChanged, this, 0);
        disconnect(this->_document->cursor(), &HexCursor::selectionChanged, this, 0);
        disconnect(this->_document->cursor(), &HexCursor::selectedPartChanged, this, 0);
        disconnect(this->_document->cursor(), &HexCursor::insertionModeChanged, this, 0);
    }

    this->_document = document;
    this->_metrics->calculate(document, this->fontMetrics());
    document->cursor()->setPosition(this->_metrics->xPosHex(), 0);

    connect(document, &HexDocument::documentChanged, [this]() {
        this->update();
    });
    connect(document->cursor(), &HexCursor::selectionChanged, [this]() {
        this->update();
    });

    connect(document->cursor(), &HexCursor::offsetChanged, [this]() {
        HexCursor* cursor = this->_document->cursor();
        this->updateCaret(cursor->offset(), cursor->nibbleIndex());

        if(!this->_metrics->ensureVisible()) {
            this->update();
        }
    });

    connect(document->cursor(), &HexCursor::blinkChanged, [this]() {
        HexCursor* cursor = this->_document->cursor();
        this->update(QRect(cursor->position(), this->_metrics->charSize()));
    });

    this->update();
}

void HexEditorPrivate::setOverwriteOnly(bool b) {
    this->_overwriteonly = b;
    if(b && this->_document != NULL) {
        HexCursor* cursor = this->_document->cursor();
        if(cursor != NULL) {
            cursor->setInsertionMode(HexCursor::OverwriteMode);
        }
    }
}

void HexEditorPrivate::setReadOnly(bool b) {
    this->_readonly = b;
}

void HexEditorPrivate::scroll(QWheelEvent* event) {
    this->wheelEvent(event);
}

void HexEditorPrivate::processDeleteEvents() {
    HexCursor* cursor = this->_document->cursor();

    if(cursor->removeSelection()) {
        return;
    }

    if(cursor->isOverwriteMode()) {
        if(cursor->isHexPartSelected()) {
            uchar hexval = this->_document->at(cursor->offset());

            if(cursor->nibbleIndex() == 1) { // Change Low Part
                hexval = (hexval & 0xF0);
            } else { // Change High Part
                hexval = (hexval & 0x0F);
            }

            this->_document->replace(cursor->offset(), hexval);
        } else {
            this->_document->replace(cursor->offset(), 0x00);
        }
    } else {
        this->_document->remove(cursor->offset(), 1);
    }
}

void HexEditorPrivate::processBackspaceEvents() {
    HexCursor* cursor = this->_document->cursor();

    if(cursor->removeSelection()) {
        return;
    }

    integer_t pos = cursor->offset() > 0 ? cursor->offset() - 1 : 0;

    if(cursor->isOverwriteMode()) {
        this->_document->replace(pos, 0x00);
    } else {
        this->_document->remove(pos, 1);
    }
}

void HexEditorPrivate::processHexPart(int key) {
    HexCursor* cursor = this->_document->cursor();
    uchar val = static_cast<uchar>(QString(key).toUInt(NULL, 16));

    cursor->removeSelection();

    if((cursor->isInsertMode()) && !cursor->nibbleIndex()) { // Insert a new byte
        this->_document->insert(cursor->offset(), val << 4); // X0 byte
        cursor->moveOffset(1, true);
        return;
    }

    if((cursor->isOverwriteMode() || cursor->nibbleIndex()) // Override mode, or update low nibble
            && cursor->offset() < this->_document->length()) {
        uchar hexval = this->_document->at(cursor->offset());

        if(cursor->nibbleIndex() == 1) { // Change Low Part
            hexval = (hexval & 0xF0) + val;
        } else { // Change High Part
            hexval = (hexval & 0x0F) + (val << 4);
        }

        this->_document->replace(cursor->offset(), hexval);
        cursor->moveOffset(1, true);
    }
}

void HexEditorPrivate::processAsciiPart(int key) {
    HexCursor* cursor = this->_document->cursor();
    cursor->removeSelection();

    if(cursor->isInsertMode()) {
        this->_document->insert(cursor->offset(), static_cast<uchar>(key));
    } else if(cursor->isOverwriteMode() && !this->_document->isEmpty()) {
        this->_document->replace(cursor->offset(), static_cast<uchar>(key));
    }

    cursor->moveOffset(1);
}

bool HexEditorPrivate::processMoveEvents(QKeyEvent* event) {
    HexCursor* cursor = this->_document->cursor();

    if(event->matches(QKeySequence::MoveToNextChar)) {
        cursor->moveOffset(1, true);
    } else if(event->matches(QKeySequence::MoveToPreviousChar)) {
        cursor->moveOffset(-1, true);
    } else if(event->matches(QKeySequence::MoveToNextLine)) {
        cursor->moveOffset(HexMetrics::BYTES_PER_LINE);
    } else if(event->matches(QKeySequence::MoveToPreviousLine)) {
        cursor->moveOffset(-HexMetrics::BYTES_PER_LINE);
    } else if(event->matches(QKeySequence::MoveToStartOfLine)) {
        cursor->moveOffset(-(cursor->offset() % HexMetrics::BYTES_PER_LINE));
    } else if(event->matches(QKeySequence::MoveToEndOfLine)) {
        cursor->setOffset(cursor->offset() | (HexMetrics::BYTES_PER_LINE - 1));
    } else if(event->matches(QKeySequence::MoveToNextPage)) {
        cursor->moveOffset((this->_metrics->visibleLines() - 1) * HexMetrics::BYTES_PER_LINE);
    } else if(event->matches(QKeySequence::MoveToPreviousPage)) {
        cursor->moveOffset(-(this->_metrics->visibleLines() - 1) * HexMetrics::BYTES_PER_LINE);
    } else if(event->matches(QKeySequence::MoveToStartOfDocument)) {
        cursor->setOffset(0);
    } else if(event->matches(QKeySequence::MoveToEndOfDocument)) {
        cursor->setOffset(this->_document->length());
    } else {
        return false;
    }

    return true;
}

bool HexEditorPrivate::processSelectEvents(QKeyEvent* event) {
    HexCursor* cursor = this->_document->cursor();

    if(event->matches(QKeySequence::SelectNextChar)) {
        cursor->moveSelection(1);
    } else if(event->matches(QKeySequence::SelectPreviousChar)) {
        cursor->moveSelection(-1);
    } else if(event->matches(QKeySequence::SelectNextLine)) {
        cursor->moveSelection(HexMetrics::BYTES_PER_LINE);
    } else if(event->matches(QKeySequence::SelectPreviousLine)) {
        cursor->moveSelection(-HexMetrics::BYTES_PER_LINE);
    } else if(event->matches(QKeySequence::SelectStartOfDocument)) {
        cursor->selectStart();
    } else if(event->matches(QKeySequence::SelectEndOfDocument)) {
        cursor->selectEnd();
    } else if(event->matches(QKeySequence::SelectAll)) {
        cursor->selectAll();
    } else {
        return false;
    }

    return true;
}

bool HexEditorPrivate::processTextInputEvents(QKeyEvent* event) {
    if(!this->_document || (event->modifiers() & Qt::ControlModifier)) {
        return false;
    }

    HexCursor* cursor = this->_document->cursor();
    int key = static_cast<int>(event->text()[0].toLatin1());

    if(cursor->isHexPartSelected() && ((key >= '0' && key <= '9') || (key >= 'a' && key <= 'f'))) { /* Check if is a Hex Char */
        this->processHexPart(key);
    } else if(cursor->isAsciiPartSelected() && (key >= 0x20 && key <= 0x7E)) { /* Check if is a Printable Char */
        this->processAsciiPart(key);
    } else if(event->key() == Qt::Key_Delete) {
        this->processDeleteEvents();
    } else if((event->key() == Qt::Key_Backspace) && (event->modifiers() == Qt::NoModifier)) {
        this->processBackspaceEvents();
    } else {
        return false;
    }

    return true;
}

bool HexEditorPrivate::processInsOvrEvents(QKeyEvent* event) {
    if((event->key() == Qt::Key_Insert) && (event->modifiers() == Qt::NoModifier)) {
        if(!this->_overwriteonly) {
            HexCursor* cursor = this->_document->cursor();
            cursor->switchMode();
            cursor->blink(true);
        }
        return true;
    }

    return false;
}

bool HexEditorPrivate::processUndoRedo(QKeyEvent* event) {
    if(event->matches(QKeySequence::Undo)) {
        this->_document->undo();
    } else if(event->matches(QKeySequence::Redo)) {
        this->_document->redo();
    } else {
        return false;
    }

    return true;
}

bool HexEditorPrivate::processClipboardKeys(QKeyEvent* event) {
    if(event->matches(QKeySequence::Cut)) {
        this->_document->cut();
    } else if(event->matches(QKeySequence::Copy)) {
        this->_document->copy();
    } else if(event->matches(QKeySequence::Paste)) {
        this->_document->paste();
    } else {
        return false;
    }

    return true;
}

void HexEditorPrivate::updateCaret(integer_t offset, integer_t nibbleindex) {
    if(!this->_document) {
        return;
    }

    HexCursor* cursor = this->_document->cursor();
    sinteger_t cursorx = 0, cursory = ((static_cast<sinteger_t>(offset) - static_cast<sinteger_t>
                                        (this->_vscrollbar->sliderPosition() * HexMetrics::BYTES_PER_LINE)) /
                                       HexMetrics::BYTES_PER_LINE) * this->_metrics->charHeight();

    if(cursor->isAddressPartSelected() || cursor->isHexPartSelected()) {
        sinteger_t x = offset % HexMetrics::BYTES_PER_LINE;
        cursorx = x * (3 * this->_metrics->charWidth()) + this->_metrics->xPosHex();

        if(nibbleindex) {
            cursorx += this->_metrics->charWidth();
        }
    } else { // AsciiPart
        sinteger_t x = offset % HexMetrics::BYTES_PER_LINE;
        cursorx = x * this->_metrics->charWidth() + this->_metrics->xPosAscii();
    }

    cursor->blink(false);
    cursor->setPosition(cursorx, cursory);
    cursor->blink(true);
}

integer_t HexEditorPrivate::offsetFromPoint(const QPoint& pt, integer_t* nibbleindex) const {
    if(!this->_document) {
        return 0;
    }

    HexCursor* cursor = this->_document->cursor();
    integer_t y = (this->_vscrollbar->sliderPosition() + (pt.y() / this->_metrics->charHeight())) * HexMetrics::BYTES_PER_LINE, x = 0;

    if(nibbleindex) {
        *nibbleindex = 0;
    }

    if(cursor->isHexPartSelected()) {
        x = (pt.x() - this->_metrics->xPosHex()) / this->_metrics->charWidth();

        if(nibbleindex && (x % 3) != 0) { // Is second nibble selected?
            *nibbleindex = 1;
        }

        x = x / 3;
    } else if(cursor->isAsciiPartSelected()) {
        x = ((pt.x() - this->_metrics->xPosAscii()) / this->_metrics->charWidth());
    }

    return y + x;
}

void HexEditorPrivate::toggleComment(const QPoint& pos) {
    integer_t offset = this->offsetFromPoint(pos);
    QString comment = this->_document->metadata()->commentString(offset);

    if(comment.isEmpty()) {
        QToolTip::hideText();
    } else {
        QToolTip::showText(this->mapToGlobal(pos), comment, this);
    }
}

void HexEditorPrivate::paintEvent(QPaintEvent* pe) {
    HexPainter hexpainter(this->_metrics, this);
    hexpainter.paint(pe, this->_theme);
}

void HexEditorPrivate::mousePressEvent(QMouseEvent* event) {
    if(!this->_document || !(event->buttons() & Qt::LeftButton)) {
        return;
    }

    QPoint pos = event->pos();

    if(!pos.x() && !pos.y()) {
        return;
    }

    HexMetrics* metrics = this->_metrics;
    HexCursor* cursor = this->_document->cursor();
    integer_t span = metrics->charHeight() / 2;

    if(pos.x() < static_cast<sinteger_t>(metrics->xPosHex() - span)) {
        cursor->setSelectedPart(HexCursor::AddressPart);
    } else if(pos.x() >= static_cast<sinteger_t>(metrics->xPosAscii() - span)) {
        cursor->setSelectedPart(HexCursor::AsciiPart);
    } else {
        cursor->setSelectedPart(HexCursor::HexPart);
    }

    integer_t nibbleidx = 0, offset = this->offsetFromPoint(pos, &nibbleidx);

    if(event->modifiers() == Qt::ShiftModifier) {
        cursor->setSelectionEnd(offset);
    } else {
        cursor->setOffset(offset, nibbleidx);
    }
}

void HexEditorPrivate::mouseMoveEvent(QMouseEvent* event) {
    if(!this->_document) {
        return;
    }

    if(event->buttons() & Qt::LeftButton) {
        HexCursor* cursor = this->_document->cursor();
        QPoint pos = event->pos();

        if(!pos.x() && !pos.y()) {
            return;
        }

        integer_t offset = this->offsetFromPoint(pos);
        cursor->setSelectionEnd(offset);
    } else if(event->buttons() == Qt::NoButton) {
        this->toggleComment(event->pos());
    }
}

void HexEditorPrivate::wheelEvent(QWheelEvent* event) {
    if(!this->_document || !this->_document->length() || (event->orientation() != Qt::Vertical)) {
        event->ignore();
        return;
    }

    HexCursor* cursor = this->_document->cursor();
    sinteger_t numdegrees = event->delta() / 8, numsteps = numdegrees / 15;
    sinteger_t maxlines = this->_document->length() / HexMetrics::BYTES_PER_LINE;
    sinteger_t pos = this->_vscrollbar->sliderPosition() - (numsteps * HexEditorPrivate::WHELL_SCROLL_LINES);

    if(pos < 0) {
        pos = 0;
    } else if(pos > maxlines) {
        pos = maxlines;
    }

    this->_vscrollbar->setSliderPosition(pos);
    this->updateCaret(cursor->offset(), cursor->nibbleIndex());
    this->update();
    event->accept();
}

void HexEditorPrivate::keyPressEvent(QKeyEvent* event) {
    if(!this->_document) {
        return;
    }

    if(this->processMoveEvents(event)) {
        return;
    }

    if(this->processSelectEvents(event)) {
        return;
    }

    if(this->processInsOvrEvents(event)) {
        return;
    }

    if(this->processUndoRedo(event)) {
        return;
    }

    if(this->processClipboardKeys(event)) {
        return;
    }

    if(!this->_readonly) {
        this->processTextInputEvents(event);
    }
}

void HexEditorPrivate::resizeEvent(QResizeEvent* e) {
    this->_metrics->calculate(this->fontMetrics()); // Update ScrollBars
    QWidget::resizeEvent(e);
}
