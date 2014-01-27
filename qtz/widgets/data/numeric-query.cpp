#include "numeric-query.h"
#include "ui_numeric-query.h"

NumericQuery::NumericQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NumericQuery),
    fieldName(QString("")),
    m_statement(QString("")),
    m_min(-100.0),
    m_max(+100.0),
    m_decimals(2)
{
    ui->setupUi(this);
    connect(ui->doubleSpinBoxEq, SIGNAL(valueChanged(double)), this,
            SLOT(makeStatement()));
    connect(ui->doubleSpinBoxGt, SIGNAL(valueChanged(double)), this,
            SLOT(makeStatement()));
    connect(ui->doubleSpinBoxLe, SIGNAL(valueChanged(double)), this,
            SLOT(makeStatement()));
    connect(ui->doubleSpinBoxB1, SIGNAL(valueChanged(double)), this,
            SLOT(makeStatement()));
    connect(ui->doubleSpinBoxB2, SIGNAL(valueChanged(double)), this,
            SLOT(makeStatement()));
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this,
            SLOT(makeStatement()));
    setMin(-100);
    setMax(100);
    setDecimals(2);
}

NumericQuery::~NumericQuery()
{
    delete ui;
}

void NumericQuery::changeEvent(QEvent *e)
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

QString NumericQuery::statement()
{
    return this->m_statement;
}

double NumericQuery::min()
{
    return this->m_min;
}

double NumericQuery::max()
{
    return this->m_max;
}

int NumericQuery::decimals()
{
    return this->m_decimals;
}

void NumericQuery::setFieldName(QString newName)
{
    bool changed = fieldName != newName;
    if(changed) {
        fieldName = newName;
        makeStatement();
        emit fieldNameChanged(newName);
    }
}

void NumericQuery::makeStatement()
{
    switch(ui->stackedWidget->currentIndex()) {
    case 0: // equal
        m_statement = QString("%1 = %2").arg(fieldName).arg(
                          ui->doubleSpinBoxEq->value());
        break;
    case 1: // greater
        m_statement = QString("%1 > %2").arg(fieldName).arg(
                          ui->doubleSpinBoxGt->value());
        break;
    case 2: // less
        m_statement = QString("%1 < %2").arg(fieldName).arg(
                          ui->doubleSpinBoxLe->value());
        break;
    case 3: // between
        m_statement = QString("%1 BETWEEN %2 AND %3").arg(fieldName).arg(
                          ui->doubleSpinBoxB1->value())
                      .arg(ui->doubleSpinBoxB2->value());
        break;
    }
    emit statementChanged(m_statement);
}

void NumericQuery::setMin(double value)
{
    m_min = value;
    ui->doubleSpinBoxEq->setMinimum(m_min);
    ui->doubleSpinBoxGt->setMinimum(m_min);
    ui->doubleSpinBoxLe->setMinimum(m_min);
    ui->doubleSpinBoxB1->setMinimum(m_min);
    ui->doubleSpinBoxB2->setMinimum(m_min);
}

void NumericQuery::setMax(double value)
{
    m_max = value;
    ui->doubleSpinBoxEq->setMaximum(m_max);
    ui->doubleSpinBoxGt->setMaximum(m_max);
    ui->doubleSpinBoxLe->setMaximum(m_max);
    ui->doubleSpinBoxB1->setMaximum(m_max);
    ui->doubleSpinBoxB2->setMaximum(m_max);
}

void NumericQuery::setDecimals(int value)
{
    m_decimals = value;
    ui->doubleSpinBoxEq->setDecimals(m_decimals);
    ui->doubleSpinBoxGt->setDecimals(m_decimals);
    ui->doubleSpinBoxLe->setDecimals(m_decimals);
    ui->doubleSpinBoxB1->setDecimals(m_decimals);
    ui->doubleSpinBoxB2->setDecimals(m_decimals);
}
