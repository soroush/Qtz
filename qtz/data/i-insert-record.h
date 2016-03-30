#ifndef IINSERTRECORD_H
#define IINSERTRECORD_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QFuture>
#include <QFutureWatcher>
#include "data.h"

class QTZ_DATA_SHARED_EXPORT IInsertRecord: public QObject
{
    Q_OBJECT
public:
    explicit IInsertRecord(QSqlTableModel* model, QObject *parent=nullptr);
    void setModel(QSqlTableModel* model);
    void setValue(const QString &name, const QVariant &val);
    bool isValid() const;
    bool isSuccessful() const;
    void insert();
    QString validateMessage();
protected slots:
    virtual void startCallback();
    virtual void finishCallback();
    virtual void validate();
protected:
    void insertWorkhorse();
    bool valid;
    bool success;
    QSqlTableModel* model;
    QSqlRecord record;
    QFuture<void> future;
    QFutureWatcher<void> watcher;
    QString m_validateMessage;
};

#endif // IINSERTRECORD_H
