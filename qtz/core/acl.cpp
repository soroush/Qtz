#include "acl.h"
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
#include <QVariant>

#include "qio.h"

QT_USE_NAMESPACE
using namespace std;

ACL *ACL::m_Instance = 0;

ACL *ACL::instance()
{
    static QMutex mutex;
    if (!m_Instance) {
        mutex.lock();
        if (!m_Instance) {
            m_Instance = new ACL;
        }
        mutex.unlock();
    }
    return m_Instance;
}

void ACL::drop()
{
    QIO::cerr << "drop called" ;
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

ACL::ACL()
{
}

bool ACL::acquireAccess(const QString &Class, const QString &Object)
{
    // TODO: add caching implementation
    QSqlQuery accessQuery;
    accessQuery.prepare("SELECT access FROM acl WHERE class=':className' AND object=':objectName'");
    accessQuery.bindValue(":className", Class);
    accessQuery.bindValue(":objectName", Object);
    if (accessQuery.exec()) {
        if (accessQuery.next()) {
            return accessQuery.value(0).toBool();
        }
        else {
            QIO::cerr << QObject::tr("Access control not defined for this item.") << endl;
            return false;
        }
    }
    else {
        QIO::cerr << QObject::tr("Unable to execute statement.") << endl;
        QIO::cerr << accessQuery.lastError().text() << endl;
        return false;
    }
}

bool ACL::acquireAccess(const QString &Object, AuthProvider *const auth)
{
    if (auth != nullptr) {
        // return acquireAccess(Object,auth->)
    }
    else {
        // TODO: throw exception
        return false;
    }
}
