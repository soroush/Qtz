#include "choose-file.hpp"
#include "ui_choose-file.h"
#include <QCompleter>

ChooseFile::ChooseFile(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ChooseFile),
    m_dialog {new QFileDialog(this)},
    m_filesystemModel {new QDirModel(this)},
    m_completer {new QCompleter(m_filesystemModel,this)} {
    ui->setupUi(this);
    ui->lineEditFileName->setCompleter(m_completer);
    connect(ui->toolButtonChoose, SIGNAL(clicked()), this, SLOT(chooseFile()));
    setViewMode(QFileDialog::List);
    setFileMode(QFileDialog::ExistingFile);
    setAcceptMode(QFileDialog::AcceptOpen);
}

ChooseFile::~ChooseFile() {
    delete ui;
    delete m_dialog;
    delete m_filesystemModel;
    delete m_completer;
}

void ChooseFile::setText(const QString& text) {
    this->ui->lineEditFileName->setText(text);
}

QString ChooseFile::text() const {
    return this->ui->lineEditFileName->text();
}

void ChooseFile::setFilter(const QString& text) {
    this->m_dialog->setNameFilter(text);
}

QString ChooseFile::filter() const {
    return this->m_dialog->nameFilters().join(";;");
}

void ChooseFile::setWindowTitle(const QString& text) {
    m_dialog->setWindowTitle(text);
}

QString ChooseFile::windowTitle() const {
    return this->m_dialog->windowTitle();
}

void ChooseFile::setViewMode(QFileDialog::ViewMode mode) {
    this->m_dialog->setViewMode(mode);
}

void ChooseFile::setFileMode(QFileDialog::FileMode mode) {
    this->m_dialog->setFileMode(mode);
}

void ChooseFile::setAcceptMode(QFileDialog::AcceptMode mode) {
    this->m_dialog->setAcceptMode(mode);
}

QFileDialog::ViewMode ChooseFile::viewMode() const {
    return m_dialog->viewMode();
}

QFileDialog::FileMode ChooseFile::fileMode() const {
    return m_dialog->fileMode();
}

QFileDialog::AcceptMode ChooseFile::acceptMode() const {
    return m_dialog->acceptMode();
}

void ChooseFile::setOption(QFileDialog::Option option, bool on) {
    m_dialog->setOption(option,on);
}

bool ChooseFile::testOption(QFileDialog::Option option) const {
    return m_dialog->testOption(option);
}

void ChooseFile::setOptions(QFileDialog::Options options) {
    m_dialog->setOptions(options);
}

QFileDialog::Options ChooseFile::options() const {
    return m_dialog->options();
}

QLineEdit* ChooseFile::lineEdit() {
    return (ui->lineEditFileName);
}

void ChooseFile::chooseFile() {
    if(m_dialog->exec() == QDialog::Accepted) {
        QString fileName = m_dialog->selectedFiles().at(0);
        ui->lineEditFileName->setText(fileName);
        emit fileSelected(fileName);
    }
}

void ChooseFile::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch(e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}
