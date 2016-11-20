#ifndef QTZ_IINSERTRECORD_HPP
#define QTZ_IINSERTRECORD_HPP

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QFuture>
#include <QFutureWatcher>
#include "data.hpp"

//class QTZ_DATA_SHARED_EXPORT IInsertRecord: public QObject {
//    Q_OBJECT
//public:
//    explicit IInsertRecord(QSqlTableModel* model, QObject* parent=nullptr);
//    ~IInsertRecord();
//    void setModel(QSqlTableModel* model);
//    QSqlTableModel* IInsertRecord::getModel();
//    void setValue(const QString& name, const QVariant& val);
//    bool isValid() const;
//    bool isSuccessful() const;
//    void insert(const QSqlRecord& record);
//    QString validateMessage();
//protected slots:
//    virtual void startCallback();
//    virtual void finishCallback();
//    virtual void validate();
//protected:
//    void insertWorkhorse();
//    bool m_valid;
//    bool m_success;
//    QSqlTableModel* m_model;
//    QSqlRecord m_record;
//    QFuture<void> m_future;
//    QFutureWatcher<void> m_watcher;
//    QString m_validateMessage;
//};

#endif // QTZ_IINSERTRECORD_HPP
