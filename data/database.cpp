#include "database.h"
#include <agt/core/settings.h>
#include <agt/core/auth-provider.h>
#include <agt/core/qio.h>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QQueue>
#include <string>

#include "table-node.h"

#include <QDebug>

using namespace std;

Database Database::instance;
bool Database::set = false;

void Database::setInstance(const QSqlDatabase &database, bool destroy)
{
    if(!set)
    {
        instance.m_database = database;
    }
    else if(destroy)
    {
        instance.m_database.close();
        instance.m_database = database;
    }
    set = true;
}

Database *Database::getInstance()
{
    if(!set)
    {
        return NULL;
    }
    return &instance;
}

QSqlDatabase *Database::database()
{
    return &instance.m_database;
}

void Database::setType(const DatabaseType &newType)
{
    m_type = newType;
}

void Database::readConnectionInfo()
{
    QString driverName = Settings::getInstance()->value("type").toString();
    if(driverName==tr("MySQL"))
    {
        instance.m_database = QSqlDatabase::addDatabase("QMYSQL");
    }
    else if(driverName==tr("SQLite 3"))
    {
        instance.m_database = QSqlDatabase::addDatabase("QSQLITE");
    }
    instance.m_database.setHostName(Settings::getInstance()->value("host").toString());
    instance.m_database.setPort(Settings::getInstance()->value("port").toInt());
    instance.m_database.setDatabaseName(Settings::getInstance()->value("database").toString());
    instance.m_database.setUserName(Settings::getInstance()->value("user").toString());
    instance.m_database.setPassword(
                AuthProvider::instance()->decryptPassword(Settings::getInstance()->value("password").toString()));
}

void Database::writeConnectionInfo()
{
    // Save setting in a normal manner (registery in Windows, Setting file in Linux and Mac OSX)
    Settings::getInstance()->setValue("host",instance.m_database.hostName());
    Settings::getInstance()->setValue("port",instance.m_database.port());
    Settings::getInstance()->setValue("database",instance.m_database.databaseName());
    Settings::getInstance()->setValue("user",instance.m_database.userName());
    Settings::getInstance()->setValue(
                "password",AuthProvider::instance()->encryptPassword(instance.m_database.password()));
}

void Database::backup(const QString &filename)
{
    QSqlQuery fetchTables;
    fetchTables.prepare("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA=DATABASE()");
    if(fetchTables.exec())
    {
        QList<TableNode*> tables;
        QQueue<TableNode*> tablesQueue;
        while(fetchTables.next())
        {
            TableNode* item = new TableNode(fetchTables.value(0).toString(),-1);
            tables << item;
            tablesQueue << item;
        }

        QSqlQuery getParents;
        QFile queryFile(":/data/resources/mysql_fk_fetch.sql");
        if(!queryFile.open(QFile::ReadOnly | QFile::Text))
        {
            QIO::qcerr << tr("Unable to open SQL query file") << endl;
            // TODO: throw new exception
        }
        QString genericQueryText = QString::fromUtf8(queryFile.readAll());
        foreach(TableNode* item, tables)
        {
            QString queryText = genericQueryText.arg(item->name);
            getParents.prepare(queryText);
            if(getParents.exec())
            {
                while(getParents.next())
                {
                    QString tableName = getParents.value(0).toString();
                    foreach(TableNode* parentItem, tables)
                    {
                        if(parentItem->name == tableName)
                        {
                            item->referencedTables << parentItem;
                            qDebug() << item->name << "referes to: " << parentItem->name;
                        }
                    }
                }
            }
            else
            {
                QIO::qcerr << tr("Unable to execute statement") << endl;
                QIO::qcerr << getParents.lastError().text() << endl;
                // TODO: throw new exception
            }
        }

        while(tables.at(0)->degreeOfFreedom == -1)
        {
            // 1. Check dependencies
            bool satisfied = true;
            foreach(TableNode* parent, tables.at(0)->referencedTables)
            {
                if(parent->degreeOfFreedom == -1)
                {
                    satisfied = false;
                    break;
                }
            }
            if(satisfied)
            {
                // 2. Write data
            }
            else
            {
                tablesQueue.enqueue(tablesQueue.dequeue());
            }
        }
        // 3. Backup tables
    }
}
