#ifndef ASYNC_QUERY_WORKER_HPP
#define ASYNC_QUERY_WORKER_HPP

#include <QObject>
#include <QtSql>
#include "database-exception.hpp"

#include "query-request.hpp"
#include "query-result.hpp"

class QueryWorker : public QObject {
        Q_OBJECT
    public:
        explicit QueryWorker(QObject *parent = 0);
        ~QueryWorker();

        static QString getSqliteConnectionName();
        static QString getMysqlConnectionName();
    signals:
        void resultsReady(const QueryResult &);
    public slots:
        void execute(const QueryRequest &);
    private:
        QSqlDatabase m_database;
        QString connectionName;
        QueryResult result;
        QSqlError error;

        void openConnection(const QueryRequest &);

        void select(const QueryRequest &);
        void update(const QueryRequest &);
        void insert(const QueryRequest &);
        void remove(const QueryRequest &);
        void startTransaction();
        void commitTransaction();
        void executeCommand(const QueryRequest &);
        void setLastRecord(const QueryRequest &);

        void lockTables(const QueryRequest &);
        void unlockTables();
};

#endif // ASYNC_QUERY_WORKER_HPP
