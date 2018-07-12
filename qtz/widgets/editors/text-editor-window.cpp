#include "text-editor-window.hpp"
#include "ui_text-editor-window.h"

#include <QFontComboBox>
#include <QLabel>

QT_USE_NAMESPACE

TextEditorWindow::TextEditorWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::textEditorWindow) {
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::Window);
    setupToolbars();
    // Connections
    connect(ui->actionUndo, SIGNAL(triggered()), ui->editor, SLOT(undo()));
    connect(ui->editor, SIGNAL(undoAvailable(bool)), ui->actionUndo,
            SLOT(setEnabled(bool)));
    connect(ui->actionRedo, SIGNAL(triggered()), ui->editor, SLOT(redo()));
    connect(ui->editor, SIGNAL(redoAvailable(bool)), ui->actionRedo,
            SLOT(setEnabled(bool)));
    connect(ui->actionCut, SIGNAL(triggered()), ui->editor, SLOT(cut()));
    connect(ui->editor, SIGNAL(copyAvailable(bool)), ui->actionCopy,
            SLOT(setEnabled(bool)));
    connect(ui->actionCopy, SIGNAL(triggered()), ui->editor, SLOT(copy()));
    connect(ui->editor, SIGNAL(copyAvailable(bool)), ui->actionCut,
            SLOT(setEnabled(bool)));
    connect(ui->actionPaste, SIGNAL(triggered()), ui->editor, SLOT(paste()));
    connect(ui->actionSelect_all, SIGNAL(triggered()), ui->editor,
            SLOT(selectAll()));
    connect(ui->actionBold, SIGNAL(triggered(bool)), this, SLOT(bold(bool)));
    connect(ui->actionItalic, SIGNAL(triggered(bool)), this, SLOT(italic(bool)));
    connect(ui->actionUnderline, SIGNAL(triggered(bool)), this,
            SLOT(underline(bool)));
    connect(ui->actionStrickout, SIGNAL(triggered(bool)), this,
            SLOT(strickout(bool)));
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)), this,
            SLOT(changeFont(QFont)));
    connect(ui->editor, SIGNAL(cursorPositionChanged()), this,
            SLOT(updateStatus()));
    connect(ui->editor, SIGNAL(undoAvailable(bool)), ui->actionUndo,
            SLOT(setEnabled(bool)));
    connect(ui->editor, SIGNAL(redoAvailable(bool)), ui->actionRedo,
            SLOT(setEnabled(bool)));
}

TextEditorWindow::~TextEditorWindow() {
    delete ui;
}

void TextEditorWindow::changeEvent(QEvent* e) {
    QMainWindow::changeEvent(e);
    switch(e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

QString TextEditorWindow::toPlainText() const {
    return ui->editor->toPlainText();
}

QString TextEditorWindow::toHtml() const {
    return ui->editor->toHtml();
}

void TextEditorWindow::setupToolbars() {
    QLabel* labelFont = new QLabel(tr("&Font:"));
    fontCombo = new QFontComboBox();
    labelFont->setBuddy(fontCombo);
    ui->toolBarFormatting->insertWidget(ui->actionBold, labelFont);
    ui->toolBarFormatting->insertWidget(ui->actionBold, fontCombo);
}

void TextEditorWindow::setText(QString value) {
    ui->editor->setText(value);
}

// Formatting

void TextEditorWindow::mergeFormatOnWordOrSelection(const QTextCharFormat
        &format) {
    QTextCursor cursor = ui->editor->textCursor();
    if(!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    ui->editor->mergeCurrentCharFormat(format);
}

void TextEditorWindow::bold(bool state) {
    QTextCharFormat f;
    f.setFontWeight(state ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(f);
}

void TextEditorWindow::italic(bool state) {
    QTextCharFormat f;
    f.setFontItalic(state);
    mergeFormatOnWordOrSelection(f);
}

void TextEditorWindow::underline(bool state) {
    QTextCharFormat f;
    f.setFontUnderline(state);
    mergeFormatOnWordOrSelection(f);
}

void TextEditorWindow::strickout(bool state) {
    QTextCharFormat f;
    f.setFontStrikeOut(state);
    mergeFormatOnWordOrSelection(f);
}

void TextEditorWindow::changeFont(QFont font) {
    QTextCharFormat format;
    format.setFontFamily(font.family());
    mergeFormatOnWordOrSelection(format);
}

void TextEditorWindow::updateStatus() {
    QTextCharFormat format = ui->editor->currentCharFormat();
    QFont font = format.font();
    ui->actionBold->setChecked(font.bold());
    ui->actionItalic->setChecked(font.italic());
    ui->actionUnderline->setChecked(font.underline());
    ui->actionStrickout->setChecked(font.strikeOut());
}
