#ifndef IINSERTRECORD_H
#define IINSERTRECORD_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QFuture>
#include <QFutureWatcher>
#include <qtz/core/library.h>

//TODO: Merge this class with IEditRecord or make a common base for these two classes.
class QTZ_SHARED_EXPORT IInsertRecord: public QObject
{
    Q_OBJECT
public:
    enum class Status{Idle, Inserting, Inserted};
    explicit IInsertRecord(QObject *parent=nullptr, QSqlTableModel* m_model=nullptr);
    virtual ~IInsertRecord();
    void setModel(QSqlTableModel* m_model);
    QSqlTableModel* getModel();
    bool isValid() const;
    bool isSuccessful() const;
    void insert(const QSqlRecord &record);
    void insertAndComplete(QSqlRecord &record);
signals:
    void started();
    void finished();
    void failed();
private slots:
    void emitFinished();
protected:
    void insertWorkhorse(const QSqlRecord &record);
    bool m_success;
    QSqlTableModel* m_model;
    QFuture<void> m_future;
    QFutureWatcher<void> m_watcher;
};

#endif // IINSERTRECORD_H
