#ifndef QTZ_DATABASE_POOL_HPP
#define QTZ_DATABASE_POOL_HPP

#include "data.hpp"
#include <QSqlDatabase>
#include <QSemaphore>
#include <QMutex>

class QTZ_DATA_SHARED_EXPORT DatabasePool
{
public:
    //DatabasePool();
    //DatabasePool(quint32 poolsize);
private:
    //QVector<QSqlDatabase*> m_databases;
    //QVector<bool> m_locks;
};

#endif // QTZ_DATABASE_POOL_HPP
