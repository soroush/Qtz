#include "date-query.h"
#include "ui_date-query.h"

DateQuery::DateQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DateQuery),
    fieldName(QString("")),
    m_statement(QString("")),
    standardFormat(QString("yyyy-MM-dd"))
{
    ui->setupUi(this);
    connect(ui->dateEditEq,SIGNAL(dateChanged(QDate)),this,SLOT(makeStatement()));
    connect(ui->dateEditGt,SIGNAL(dateChanged(QDate)),this,SLOT(makeStatement()));
    connect(ui->dateEditLe,SIGNAL(dateChanged(QDate)),this,SLOT(makeStatement()));
    connect(ui->dateEditB1,SIGNAL(dateChanged(QDate)),this,SLOT(makeStatement()));
    connect(ui->dateEditB2,SIGNAL(dateChanged(QDate)),this,SLOT(makeStatement()));
    connect(ui->stackedWidget,SIGNAL(currentChanged(int)),this,SLOT(makeStatement()));
}

DateQuery::~DateQuery()
{
    delete ui;
}

void DateQuery::changeEvent(QEvent *e)
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

QString DateQuery::statement() const
{
    return this->m_statement;
}

void DateQuery::setFieldName(QString newName)
{
    bool changed = fieldName != newName;
    if(changed)
    {
        fieldName = newName;
        makeStatement();
        emit fieldNameChanged(newName);
    }
}

void DateQuery::makeStatement()
{
    switch(ui->stackedWidget->currentIndex())
    {
    case 0: // equal
        m_statement = QString("%1 = '%2'").arg(fieldName).arg(ui->dateEditEq->date().toString(Qt::ISODate));
        break;
    case 1: // greater
        m_statement = QString("%1 > '%2'").arg(fieldName).arg(ui->dateEditGt->date().toString(Qt::ISODate));
        break;
    case 2: // less
        m_statement = QString("%1 < '%2'").arg(fieldName).arg(ui->dateEditLe->date().toString(Qt::ISODate));
        break;
    case 3: // between
        m_statement = QString("%1 BETWEEN '%2' AND '%3'").arg(fieldName)
                .arg(ui->dateEditB1->date().toString(Qt::ISODate))
                .arg(ui->dateEditB2->date().toString(Qt::ISODate));
        break;
    }
    emit statementChanged(m_statement);
}

