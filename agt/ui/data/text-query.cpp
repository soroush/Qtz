#include "text-query.h"
#include "ui_text-query.h"

TextQuery::TextQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextQuery)
{
    ui->setupUi(this);
    connect(ui->lineEditContains,SIGNAL(textChanged(QString)),this,SLOT(makeStatement()));
    connect(ui->lineEditEquals,SIGNAL(textChanged(QString)),this,SLOT(makeStatement()));
    connect(ui->stackedWidget,SIGNAL(currentChanged(int)),this,SLOT(makeStatement()));
    connect(ui->lineEditContains,SIGNAL(returnPressed()),this,SLOT(pressReturn()));
    connect(ui->lineEditEquals,SIGNAL(returnPressed()),this,SLOT(pressReturn()));

}

TextQuery::~TextQuery()
{
    delete ui;
}

void TextQuery::changeEvent(QEvent *e)
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

QString TextQuery::statement()
{
    return this->m_statement;
}

QString TextQuery::inputMask()
{
    return this->m_inputMask;
}

void TextQuery::setFieldName(QString newName)
{
    bool changed = fieldName != newName;
    fieldName = newName;
    if(changed)
    {
        makeStatement();
        emit fieldNameChanged(newName);
    }
}

void TextQuery::setInputMask(QString mask)
{
    bool changed = mask == m_inputMask;
    this->m_inputMask = mask;
    ui->lineEditContains->setInputMask(mask);
    if(changed) emit inputMaskChanged(mask);
}

void TextQuery::makeStatement()
{
    switch(ui->stackedWidget->currentIndex())
    {
    case 0: // contains
        m_statement = QString("%1 LIKE '%%2%'").arg(fieldName).arg(ui->lineEditContains->text());
        break;
    case 1: // equal
        m_statement = QString("%1 = '%2'").arg(fieldName).arg(ui->lineEditEquals->text());
        break;
    }
    emit statementChanged(m_statement);
}

void TextQuery::pressReturn()
{
    emit returnPressed();
}
