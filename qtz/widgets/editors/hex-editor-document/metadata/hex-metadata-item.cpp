#include "hex-metadata-item.hpp"

HexMetadataItem::HexMetadataItem(integer_t startoffset,
                                   integer_t endoffset,
                                   QObject* parent) :
    QObject(parent),
    _startoffset(startoffset),
    _endoffset(endoffset) {
}

integer_t HexMetadataItem::startOffset() const {
    return this->_startoffset;
}

integer_t HexMetadataItem::endOffset() const {
    return this->_endoffset;
}

bool HexMetadataItem::contains(integer_t offset) const {
    return (offset >= this->_startoffset) && (offset <= this->_endoffset);
}

bool HexMetadataItem::hasBackColor() const {
    return this->_backcolor.isValid();
}

bool HexMetadataItem::hasForeColor() const {
    return this->_forecolor.isValid();
}

bool HexMetadataItem::hasComment() const {
    return !this->_comment.isEmpty();
}

const QColor& HexMetadataItem::backColor() const {
    return this->_backcolor;
}

const QColor& HexMetadataItem::foreColor() const {
    return this->_forecolor;
}

const QString& HexMetadataItem::comment() const {
    return this->_comment;
}

void HexMetadataItem::setBackColor(const QColor& c) {
    this->_backcolor = c;
}

void HexMetadataItem::setForeColor(const QColor& c) {
    this->_forecolor = c;
}

void HexMetadataItem::setComment(const QString& s) {
    this->_comment = s;
}

void HexMetadataItem::clearComment() {
    this->_comment.clear();
}

void HexMetadataItem::clearColors() {
    this->_backcolor = QColor();
    this->_forecolor = QColor();
}
