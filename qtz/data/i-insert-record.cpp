#include "i-insert-record.h"
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif#include <qtz/data/database.h>

IInsertRecord::IInsertRecord(QSqlTableModel *_model, QObject *parent):
    QObject(parent)
{
    this->setModel(_model);
    connect(&this->watcher,SIGNAL(finished()),this,SLOT(finishCallback()));
}

void IInsertRecord::setModel(QSqlTableModel *_model)
{
    this->model = _model;
    this->record = this->model->record();
}

void IInsertRecord::setValue(const QString &name, const QVariant &val)
{
    this->record.setValue(name,val);
    this->validate();
}

bool IInsertRecord::isValid() const
{
    return this->valid;
}

bool IInsertRecord::isSuccessful() const
{
    return this->success;
}

void IInsertRecord::insert()
{
    startCallback();
    future = QtConcurrent::run(this,&IInsertRecord::insertWorkhorse);
    watcher.setFuture(future);
}

QString IInsertRecord::validateMessage()
{
    return this->m_validateMessage;
}

void IInsertRecord::startCallback()
{
    // TODO: Maybe throug exception?
}

void IInsertRecord::finishCallback()
{
    // TODO: Maybe throug exception?
}

void IInsertRecord::validate()
{
    // TODO: Maybe throug exception?
}

void IInsertRecord::insertWorkhorse()
{
    success = this->model->insertRecord(-1,this->record);
    this->model->submitAll();
}
