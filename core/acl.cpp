#include "acl.h"
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
#include <QVariant>

#include "qio.h"

using namespace std;

ACL* ACL::m_Instance = 0;

ACL *ACL::instance()
{
    static QMutex mutex;
    if (!m_Instance)
    {
        mutex.lock();
        if (!m_Instance)
            m_Instance = new ACL;
        mutex.unlock();
    }
    return m_Instance;
}

void ACL::drop()
{
    QIO::qcerr << "drop called" ;
//    QMutex mutex;
//    mutex.lock();
////    delete m_Instance;
//    mutex.unlock();
}

void ACL::setDataAdapter(const QString &_objectsTable,
                         const QString &_classesTable,
                         const QString &_listsTable)
{
//    this->database = _database;
    objectsTable = _objectsTable;
    classesTable = _classesTable;
    listsTable = _listsTable;
}

bool ACL::acquireAccess(const QString &Class, const QString &Object)
{
    // TODO: add caching implementation

    QString queryText = QString("SELECT %1 from %2 where %3='%4' and %5='%6'")
            .arg("access")
            .arg("acl")
            .arg("class")
            .arg(Class)
            .arg("object")
            .arg(Object);
    QSqlQuery accessQuery;
    accessQuery.prepare(queryText);
    if(accessQuery.exec())
    {
        if(accessQuery.next())
        {
            return accessQuery.value(0).toBool();
        }
        else
        {
            QIO::qcerr << QObject::tr("Access control not defined for this item") << endl;
            return false;
        }
    }
    else
    {
        QIO::qcerr << QObject::tr("Unable to execute statement") << endl;
        QIO::qcerr << accessQuery.lastError().text() << endl;
        return false;
    }

}

bool ACL::acquireAccess(const QString &Object, AuthProvider *const auth)
{
    if(auth != NULL)
    {
//        return acquireAccess(Object,auth)
    }
}

ACL::ACL()
{
}

ACL::ACL(const ACL &)
{
}

ACL &ACL::operator =(const ACL &)
{
}

ACL::~ACL()
{
}
