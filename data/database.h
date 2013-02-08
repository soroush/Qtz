#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <agt/global.h>

class AGTSHARED_EXPORT Database : public QObject {
    Q_OBJECT
private:
    Database(QObject* parent=NULL):
        QObject(parent){}
    Database(const Database& other):
        QObject(other.parent()){}
public:
    static void setInstance(const QSqlDatabase &database, bool destroy=false);
    static Database *getInstance();
    QSqlDatabase *database();
    enum DatabaseType{MySQL=0x01, SQLite=0x02};
    void setType(const DatabaseType&);
    DatabaseType type();
    void readConnectionInfo();
    void writeConnectionInfo();

    void backup(const QString& filename);

private:
    static Database instance;
    static bool set;
    QSqlDatabase m_database;
    DatabaseType m_type;
};

#endif // DATABASE_H
