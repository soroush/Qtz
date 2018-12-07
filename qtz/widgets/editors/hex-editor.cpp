#include "hex-editor.hpp"
#include "hex-editor-private.hpp"

HexEditor::HexEditor(QWidget* parent): QFrame(parent) {
    this->_vscrollbar = new QScrollBar(Qt::Vertical);
    this->_scrollarea = new QScrollArea();
    this->_hexedit_p = new HexEditorPrivate(this->_scrollarea, this->_vscrollbar);

    connect(this->_hexedit_p, &HexEditorPrivate::verticalScroll, this, &HexEditor::verticalScroll);
    connect(this->_hexedit_p, &HexEditorPrivate::visibleLinesChanged, this, &HexEditor::visibleLinesChanged);

    this->_scrollarea->setFocusPolicy(Qt::NoFocus);
    this->_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Do not show vertical QScrollBar
    this->_scrollarea->setFrameStyle(QFrame::NoFrame);
    this->_scrollarea->setWidgetResizable(true);
    this->_scrollarea->setWidget(this->_hexedit_p);

    this->setFocusPolicy(Qt::NoFocus);
    this->setFocusProxy(this->_hexedit_p);

    this->_hlayout = new QHBoxLayout();
    this->_hlayout->setSpacing(0);
    this->_hlayout->setMargin(0);
    this->_hlayout->addWidget(this->_scrollarea);
    this->_hlayout->addWidget(this->_vscrollbar);

    this->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    this->setLayout(this->_hlayout);
}

HexDocument* HexEditor::document() const {
    return this->_hexedit_p->document();
}

HexMetrics* HexEditor::metrics() const {
    return this->_hexedit_p->metrics();
}

bool HexEditor::overwriteOnly() const {
    return this->_hexedit_p->overwriteOnly();
}

void HexEditor::setOverwriteOnly(bool b) {
    this->_hexedit_p->setOverwriteOnly(b);
}

bool HexEditor::readOnly() const {
    return this->_hexedit_p->readOnly();
}

void HexEditor::setReadOnly(bool b) {
    this->_hexedit_p->setReadOnly(b);
}

void HexEditor::setDocument(HexDocument* document) {
    this->_hexedit_p->setDocument(document);
}

void HexEditor::scroll(QWheelEvent* event) {
    this->_hexedit_p->scroll(event);
}
