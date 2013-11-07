#ifndef ACL_H
#define ACL_H

#include <QMutex>
#include <QSqlDatabase>
#include "library.h"
#include "auth-provider.h"

class QTZ_SHARED_EXPORT ACL {
private:
    ACL();
    ACL(const ACL &);
    ACL &operator=(const ACL &);
    ~ACL();

    static ACL *m_Instance;
    QString objectsTable;
    QString classesTable;
    QString listsTable;
    bool acquireAccess(const QString &Class,
                       const QString &Object);
public:
    void setDataAdapter(const QString &objectsTable = "acl_objects",
                        const QString &classesTable = "acl_classes",
                        const QString &listsTable = "acl");
    QString currentIdentityClass();
    bool acquireAccess(const QString &Object, AuthProvider *const auth = NULL);
    static ACL *instance();
    static void drop();

    enum SourceType {
        Database
    };
};

#endif // ACL_H
