#include "i-insert-record.h"
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif
#include <qtz/data/database.h>
#include <QDebug>

IInsertRecord::IInsertRecord(QObject *parent, QSqlTableModel *_model):
    QObject(parent), m_model(_model)
{
    connect(&this->m_watcher,SIGNAL(finished()),this,SLOT(emitFinished()));
}

IInsertRecord::~IInsertRecord()
{
}

void IInsertRecord::setModel(QSqlTableModel *_model)
{
    this->m_model = _model;
}

QSqlTableModel *IInsertRecord::getModel()
{
    return this->m_model;
}

bool IInsertRecord::isSuccessful() const
{
    return this->m_success;
}

void IInsertRecord::insert(const QSqlRecord& record)
{
    emit started();
    m_success = this->m_model->insertRecord(-1,record);
    this->m_model->submitAll();
    if(!m_success){
        emit failed();
    }
    emit finished();
}

void IInsertRecord::insertAndComplete(QSqlRecord &record)
{
    // FIXME: Following assumptions are applied:
    // 1. There is only one auto-increment field
    // 2. The AI field is first field of record
    // 3. The AI field is name "id"
    emit started();
    m_success = this->m_model->insertRecord(-1,record);
    this->m_model->submitAll();
    if(!m_success){
        emit failed();
    }
    else {
        QVariant id = m_model->query().lastInsertId();
        if(id.isValid() && !id.isNull()){
            record.setValue(0,id);
        }
        else{
            emit failed();
        }
    }
}

void IInsertRecord::emitFinished()
{
    emit finished();
}

void IInsertRecord::insertWorkhorse(const QSqlRecord &record)
{
    m_success = this->m_model->insertRecord(-1,record);
    this->m_model->submitAll();
    if(!m_success){
        emit failed();
    }
}
