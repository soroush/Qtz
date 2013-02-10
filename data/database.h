#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QList>
#include <QQueue>
#include <QSqlDatabase>
#include <agt/global.h>

struct TableNode;

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
    void setBlockSize(const unsigned int& size);
    unsigned int blockSize() const;

    void backup(const QString& filename);

signals:
    void completed(double);
    void completed();

private:
    static Database instance;
    static bool set;

    QSqlDatabase m_database;
    DatabaseType m_type;
    unsigned int m_blockSize;
    uint getNumberOfDBRows();
    uint getNumberOfTableRows(const QString& tableName);
    uint getNumberOfTableColumns(const QString& tableName);
    void getTables(QQueue<TableNode *> &outputList);
    void getParents(const QQueue<TableNode*>& input);
    void sortTables(QQueue<TableNode *> &input, QQueue<TableNode*>& output);
};

#endif // DATABASE_H
