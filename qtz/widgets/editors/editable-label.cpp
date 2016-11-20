#include "editable-label.hpp"
#include "ui_editable-label.h"
#include "text-editor-window.hpp"
#include <QVBoxLayout>

EditableLabel::EditableLabel(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::EditableLabel),
    currentStatus(Normal),
    mainLayout(new QVBoxLayout(this)) {
    m_label.setAlignment(Qt::AlignTop);
    mainLayout->addWidget(&m_label);
    setLayout(mainLayout);
    ui->setupUi(this);
}

EditableLabel::~EditableLabel() {
    delete ui;
}

void EditableLabel::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch(e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void EditableLabel::startEditig() {
    if(currentStatus == Normal) {
        this->currentStatus = Editing;
        QString temp = m_label.text();
        m_label.setHidden(true);
        editor = new TextEditorWindow();
        mainLayout->addWidget(editor);
        editor->setText(temp);
    }
}

void EditableLabel::finishEditig() {
    if(currentStatus == Editing) {
        this->currentStatus = Normal;
        QString temp = editor->toHtml();
        editor->setHidden(true);
        m_label.setText(temp);
        m_label.setHidden(false);
        delete editor;
    }
}
