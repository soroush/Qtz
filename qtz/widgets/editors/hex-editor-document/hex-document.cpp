#include "hex-document.hpp"
#include "commands/insert-command.hpp"
#include "commands/remove-command.hpp"
#include "commands/replace-command.hpp"
#include <QGuiApplication>
#include <QClipboard>
#include <QFileInfo>
#include <QBuffer>
#include <QFile>

HexDocument::HexDocument(QIODevice* device, QObject* parent): QObject(parent), _baseaddress(0) {
    this->_gapbuffer = new GapBuffer(device);
    this->_cursor = new HexCursor(this);
    this->_metadata = new HexMetadata(this);

    connect(this->_metadata, &HexMetadata::metadataChanged, this, &HexDocument::documentChanged);
    connect(&this->_undostack, &QUndoStack::canUndoChanged, this, &HexDocument::canUndoChanged);
    connect(&this->_undostack, &QUndoStack::canRedoChanged, this, &HexDocument::canRedoChanged);
}

HexDocument::~HexDocument() {
    delete this->_gapbuffer;
    this->_gapbuffer = NULL;
}

HexCursor* HexDocument::cursor() const {
    return this->_cursor;
}

HexMetadata* HexDocument::metadata() const {
    return this->_metadata;
}

integer_t HexDocument::length() const {
    return this->_gapbuffer->length();
}

integer_t HexDocument::baseAddress() const {
    return this->_baseaddress;
}

bool HexDocument::canUndo() const {
    return this->_undostack.canUndo();
}

bool HexDocument::canRedo() const {
    return this->_undostack.canRedo();
}

QByteArray HexDocument::read(integer_t offset, integer_t len) {
    return this->_gapbuffer->read(offset, len);
}

QByteArray HexDocument::selectedBytes() const {
    if(!this->_cursor->hasSelection()) {
        return QByteArray();
    }

    return this->_gapbuffer->read(this->_cursor->selectionStart(), this->_cursor->selectionEnd());
}

char HexDocument::at(integer_t offset) const {
    return this->_gapbuffer->at(offset);
}

void HexDocument::setBaseAddress(integer_t baseaddress) {
    if(this->_baseaddress == baseaddress) {
        return;
    }

    this->_baseaddress = baseaddress;
    emit baseAddressChanged();
}

HexDocument* HexDocument::fromDevice(QIODevice* iodevice) {
    if(!iodevice->isOpen()) {
        iodevice->open(QFile::ReadWrite);
    }

    return new HexDocument(iodevice);
}

HexDocument* HexDocument::fromFile(QString filename) {
    QFileInfo fi(filename);
    QFile* f = new QFile(filename);

    if(fi.isWritable()) {
        f->open(QFile::ReadWrite);
    } else {
        f->open(QFile::ReadOnly);
    }

    HexDocument* document = HexDocument::fromDevice(f);
    f->setParent(document);
    return document;
}

HexDocument* HexDocument::fromMemory(const char* data, integer_t length) {
    return HexDocument::fromMemory(QByteArray::fromRawData(data, length));
}

HexDocument* HexDocument::fromMemory(const QByteArray& ba) {
    QBuffer* b = new QBuffer();
    b->setData(ba);
    b->open(QFile::ReadOnly);

    HexDocument* document = HexDocument::fromDevice(b);
    b->setParent(document);
    return document;
}

void HexDocument::undo() {
    this->_undostack.undo();
    emit documentChanged();
}

void HexDocument::redo() {
    this->_undostack.redo();
    emit documentChanged();
}

void HexDocument::cut() {
    if(!this->_cursor->hasSelection()) {
        return;
    }

    QClipboard* c = qApp->clipboard();
    c->setText(QString(this->selectedBytes()));
    this->_cursor->removeSelection();
}

void HexDocument::copy() {
    if(!this->_cursor->hasSelection()) {
        return;
    }

    QClipboard* c = qApp->clipboard();
    c->setText(QString(this->selectedBytes()));
}

void HexDocument::paste() {
    QClipboard* c = qApp->clipboard();
    QByteArray data = c->text().toUtf8();

    if(data.isEmpty()) {
        return;
    }

    this->_cursor->removeSelection();

    if(this->_cursor->isInsertMode()) {
        this->insert(this->_cursor->offset(), data);
    } else {
        this->replace(this->_cursor->offset(), data);
    }

    this->_cursor->moveOffset(data.length());
}

void HexDocument::insert(integer_t offset, uchar b) {
    this->insert(offset, QByteArray(1, b));
}

void HexDocument::replace(integer_t offset, uchar b) {
    this->replace(offset, QByteArray(1, b));
}

void HexDocument::insert(integer_t offset, const QByteArray& data) {
    this->_undostack.push(new InsertCommand(this->_gapbuffer, offset, data));
    emit documentChanged();
}

void HexDocument::replace(integer_t offset, const QByteArray& data) {
    this->_undostack.push(new ReplaceCommand(this->_gapbuffer, offset, data));
    emit documentChanged();
}

void HexDocument::remove(integer_t offset, integer_t len) {
    this->_undostack.push(new RemoveCommand(this->_gapbuffer, offset, len));
    emit documentChanged();
}

void HexDocument::highlightFore(integer_t startoffset, integer_t endoffset, const QColor& c) {
    if((startoffset == endoffset) || !c.isValid()) {
        return;
    }

    HexMetadataItem* metaitem = new HexMetadataItem(startoffset, endoffset, this->_metadata);
    metaitem->setForeColor(c);
    this->_metadata->insert(metaitem);
}

void HexDocument::highlightBack(integer_t startoffset, integer_t endoffset, const QColor& c) {
    if((startoffset == endoffset) || !c.isValid()) {
        return;
    }

    HexMetadataItem* metaitem = new HexMetadataItem(startoffset, endoffset, this->_metadata);
    metaitem->setBackColor(c);
    this->_metadata->insert(metaitem);
}

void HexDocument::highlightForeRange(integer_t offset, integer_t length, const QColor& c) {
    this->highlightFore(offset, offset + length - 1, c);
}

void HexDocument::highlightBackRange(integer_t offset, integer_t length, const QColor& c) {
    this->highlightBack(offset, offset + length - 1, c);
}

void HexDocument::comment(integer_t startoffset, integer_t endoffset, const QString& s) {
    if((startoffset == endoffset) || s.isEmpty()) {
        return;
    }

    HexMetadataItem* metaitem = new HexMetadataItem(startoffset, endoffset, this->_metadata);
    metaitem->setComment(s);
    this->_metadata->insert(metaitem);
}

void HexDocument::commentRange(integer_t offset, integer_t length, const QString& s) {
    this->comment(offset, offset + length - 1, s);
}

void HexDocument::clearHighlighting() {
    this->_metadata->clearHighlighting();
}

void HexDocument::clearComments() {
    this->_metadata->clearComments();
}

void HexDocument::clearMetadata() {
    this->beginMetadata();
    this->_metadata->clearComments();
    this->_metadata->clearHighlighting();
    this->endMetadata();
}

void HexDocument::beginMetadata() {
    this->_metadata->beginMetadata();
}

void HexDocument::endMetadata() {
    this->_metadata->endMetadata();
}

QByteArray HexDocument::read(integer_t offset, integer_t len) const {
    return this->_gapbuffer->read(offset, len);
}

bool HexDocument::saveTo(QIODevice* device) {
    if(!device->isWritable()) {
        return false;
    }

    device->write(this->_gapbuffer->toByteArray());
    return true;
}

bool HexDocument::isEmpty() const {
    return this->_gapbuffer->length() <= 0;
}
