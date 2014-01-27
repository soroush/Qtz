#include "data-navigator.h"
#include "ui_data-navigator.h"

DataNavigator::DataNavigator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataNavigator)
{
    ui->setupUi(this);
}

DataNavigator::~DataNavigator()
{
    delete ui;
}

void DataNavigator::changeEvent(QEvent *e)
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

void DataNavigator::setModel(QSqlTableModel *_model)
{
    this->m_model = _model;
}

void DataNavigator::setRowCount(const size_t &count)
{
    this->m_rowCount = count;
}

QSqlTableModel *DataNavigator::model() const
{
    return this->m_model;
}

size_t DataNavigator::rowCount() const
{
    return this->m_rowCount;
}
