#include "hex-cursor.hpp"
#include "hex-document.hpp"
#include <QWidget>
#include <QPoint>

#define currentDocument static_cast<HexDocument*>(this->parent())
//#define currentContainer static_cast<QWidget*>(currentDocument->parent())

const int HexCursor::CURSOR_BLINK_INTERVAL = 500; // 5ms

HexCursor::HexCursor(QObject* parent) : QObject(parent), _selectedpart(HexCursor::HexPart),
    _insertionmode(HexCursor::OverwriteMode) {
    this->_cursorx = this->_cursory = 0;
    this->_selectionstart = this->_offset = this->_nibbleindex = 0;
    this->_timerid = this->startTimer(HexCursor::CURSOR_BLINK_INTERVAL);
}

HexCursor::~HexCursor() {
    this->killTimer(this->_timerid);
}

QPoint HexCursor::position() const {
    return QPoint(this->_cursorx, this->_cursory);
}

sinteger_t HexCursor::cursorX() const {
    return this->_cursorx;
}

sinteger_t HexCursor::cursorY() const {
    return this->_cursory;
}

integer_t HexCursor::offset() const {
    return this->_offset;
}

integer_t HexCursor::nibbleIndex() const {
    return this->_nibbleindex;
}

integer_t HexCursor::selectionStart() const {
    return qMin(this->_offset, this->_selectionstart);
}

integer_t HexCursor::selectionEnd() const {
    return qMax(this->_offset, this->_selectionstart);
}

integer_t HexCursor::selectionLength() const {
    return this->selectionEnd() - this->selectionStart();
}

HexCursor::SelectedPart HexCursor::selectedPart() const {
    return this->_selectedpart;
}

HexCursor::InsertionMode HexCursor::insertionMode() const {
    return this->_insertionmode;
}

bool HexCursor::isAddressPartSelected() const {
    return this->_selectedpart == HexCursor::AddressPart;
}

bool HexCursor::isHexPartSelected() const {
    return this->_selectedpart == HexCursor::HexPart;
}

bool HexCursor::isAsciiPartSelected() const {
    return this->_selectedpart == HexCursor::AsciiPart;
}

bool HexCursor::isInsertMode() const {
    return this->_insertionmode == HexCursor::InsertMode;
}

bool HexCursor::isOverwriteMode() const {
    return this->_insertionmode == HexCursor::OverwriteMode;
}

bool HexCursor::hasSelection() const {
    return this->_offset != this->_selectionstart;
}

bool HexCursor::blinking() const {
    return this->_blink;
}

bool HexCursor::isSelected(integer_t offset) const {
    if(this->_offset == this->_selectionstart) {
        return false;
    }

    return (offset >= this->selectionStart()) && (offset < this->selectionEnd());
}

void HexCursor::selectStart() {
    this->setOffset(0);
}

void HexCursor::selectEnd() {
    this->setOffset(currentDocument->length());
}

void HexCursor::selectAll() {
    this->setSelection(0, currentDocument->length());
}

void HexCursor::setPosition(sinteger_t x, sinteger_t y) {
    if((this->_cursorx == x) && (this->_cursory == y)) {
        return;
    }

    this->_cursorx = x;
    this->_cursory = y;
    emit positionChanged();
}

void HexCursor::setOffset(integer_t offset) {
    this->setOffset(offset, 0);
}

void HexCursor::setOffset(integer_t offset, integer_t nibbleindex) {
    offset = qMin(offset, currentDocument->length()); // Check EOF
    this->_selectionstart = offset;
    this->_nibbleindex = nibbleindex;

    this->setSelectionEnd(offset);
}

void HexCursor::setSelectionEnd(integer_t offset) {
    offset = qMin(offset, currentDocument->length()); // Check EOF
    this->_offset = offset;

    emit selectionChanged();
    emit offsetChanged();
}

void HexCursor::setSelection(integer_t startoffset, integer_t endoffset) {
    this->setOffset(startoffset);
    this->setSelectionEnd(endoffset);
}

void HexCursor::setSelectionRange(integer_t startoffset, integer_t length) {
    this->setSelection(startoffset, startoffset + length);
}

void HexCursor::setSelectedPart(HexCursor::SelectedPart sp) {
    if(sp == this->_selectedpart) {
        return;
    }

    this->_selectedpart = sp;
    emit selectedPartChanged();
}

void HexCursor::setInsertionMode(HexCursor::InsertionMode mode) {
    bool differentmode = (this->_insertionmode != mode);
    this->_insertionmode = mode;

    if(differentmode) {
        emit insertionModeChanged();
    }
}

void HexCursor::clearSelection() {
    if(this->_selectionstart == this->_offset) {
        return;
    }

    this->_selectionstart = this->_offset;
    emit selectionChanged();
}

bool HexCursor::removeSelection() {
    if(!this->hasSelection()) {
        return false;
    }

    currentDocument->remove(this->selectionStart(), this->selectionLength());
    this->clearSelection();
    return true;
}

void HexCursor::moveOffset(sinteger_t c, bool bynibble) {
    if(!c) {
        return;
    }

    if(qAbs(c) > 1) {
        bynibble = false;
    }

    integer_t nindex = 0;

    if(bynibble) {
        if(!this->_nibbleindex) {
            if(c > 0) {
                c = 0;
            }

            nindex = 1;
        } else {
            if(c < 0) {
                c = 0;
            }

            nindex = 0;
        }
    }

    integer_t offset = this->_offset + c;

    if(offset >= currentDocument->length()) {
        offset = c > 0 ? currentDocument->length() : 0;
    }

    this->setOffset(offset, nindex);
}

void HexCursor::moveSelection(sinteger_t c) {
    if(!c) {
        return;
    }

    integer_t offset = this->_offset + c;

    if(offset >= currentDocument->length()) {
        offset = c > 0 ? currentDocument->length() : 0;
    }

    this->setSelectionEnd(offset);
}

void HexCursor::blink(bool b) {
    this->_blink = b;
    emit blinkChanged();
}

void HexCursor::switchMode() {
    if(this->_insertionmode == HexCursor::OverwriteMode) {
        this->_insertionmode = HexCursor::InsertMode;
    } else {
        this->_insertionmode = HexCursor::OverwriteMode;
    }

    emit insertionModeChanged();
}

void HexCursor::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event);

    this->_blink = !this->_blink;
    emit blinkChanged();
}
