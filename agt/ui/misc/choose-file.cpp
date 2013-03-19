#include "choose-file.h"
#include "ui_choose-file.h"

#include <QFileDialog>

ChooseFile::ChooseFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseFile)
{
    ui->setupUi(this);
    connect(ui->toolButtonChoose,SIGNAL(clicked()), this, SLOT(chooseFile()));
}

ChooseFile::~ChooseFile()
{
    delete ui;
}

void ChooseFile::setLabelText(const QString &text)
{
    this->ui->label->setText(text);
}

QString ChooseFile::labelText()
{
    return this->ui->label->text();
}

void ChooseFile::setText(const QString &text)
{
    this->ui->lineEdit->setText(text);
}

QString ChooseFile::text()
{
    return this->ui->lineEdit->text();
}

void ChooseFile::setFilter(const QString &text)
{
    this->m_filter = text;
}

QString ChooseFile::filter()
{
    return this->m_filter;
}

void ChooseFile::setWindowTitle(const QString &text)
{
    this->m_windowTitle = text;
}

QString ChooseFile::windowTitle()
{
    return this->m_windowTitle;
}

QLineEdit* ChooseFile::lineEdit()
{
    return (ui->lineEdit);
}

void ChooseFile::chooseFile()
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen); // TODO: add more flexibility here
    dialog.setFileMode(QFileDialog::ExistingFile); // TODO: add more flexibility here
    dialog.setNameFilter(this->m_filter);
    dialog.setWindowTitle(this->m_windowTitle);
    if(dialog.exec()==QDialog::Accepted)
    {
        QString fileName = dialog.selectedFiles().at(0);
        ui->lineEdit->setText(fileName);
        emit fileSelected(fileName);
    }
}

void ChooseFile::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
