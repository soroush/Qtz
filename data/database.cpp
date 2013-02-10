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
    getInstance()->setBlockSize(100);
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

void Database::setBlockSize(const unsigned int &size)
{
    this->m_blockSize = size;
}

unsigned int Database::blockSize() const
{
    return this->m_blockSize;
}

void Database::backup(const QString &filename)
{
    QQueue<TableNode*> tablesQueue;
    getTables(tablesQueue);
    getParents(tablesQueue);
    QQueue<TableNode*> orderedQueue;
    sortTables(tablesQueue,orderedQueue);

    // Starting task
    QFile backupFile(filename);
    if(backupFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        // Prepare
        uint totalRows = getNumberOfDBRows();
        uint writtenRows = 0;

        QDataStream out(&backupFile);

        // Write database information:
        out << Database::getInstance()->database()->databaseName(); // Stored as QString
        out << quint32(orderedQueue.size());

        foreach(TableNode* table, orderedQueue)
        {
            uint fieldCount = getNumberOfTableColumns(table->name);
            uint rowCount = getNumberOfTableRows(table->name);
            out << table->name;
            out << (rowCount);
            uint blockCount = rowCount/blockSize();
            if(rowCount%blockSize() != 0)
                ++blockCount;
            QString selectQueryText =  QString("SELECT * FROM %1 LIMIT %2,%3")
                    .arg(table->name,"%1",QString::number(blockSize()));
            QSqlQuery selectQuery;
            for (uint i = 0; i < blockCount; ++i)
            {
                QString selectQueryText2 = selectQueryText.arg(i*blockSize());
                selectQuery.prepare(selectQueryText2);
                if(selectQuery.exec())
                {
                    while(selectQuery.next())
                    {
                        for (uint f = 0; f < fieldCount; ++f) {
                            out << selectQuery.value(f);
                        }
                        ++writtenRows;
                    }
                    emit completed(100.0*static_cast<double>(writtenRows)/static_cast<double>(totalRows));
                }
                else
                {
                }
            }
        }

    }
    else
    {
        QIO::cerr << tr("Unable to open backup file for writing.") << endl;
    }
}


uint Database::getNumberOfDBRows()
{
    QSqlQuery query;
    if( query.exec("SELECT SUM(TABLE_ROWS) "
                   "FROM INFORMATION_SCHEMA.TABLES "
                   "WHERE TABLE_SCHEMA = DATABASE()"))
    {
        query.next();
        uint result = query.value(0).toUInt();
        return result;
    }
    return 0;
}

uint Database::getNumberOfTableRows(const QString &tableName)
{
    QString countQueryText = QString("SELECT COUNT(*) from %1").arg(tableName);
    QSqlQuery countQuery;
    countQuery.prepare(countQueryText);
    if(countQuery.exec())
    {
        countQuery.next();
        uint result = countQuery.value(0).toUInt();
        return result;
    }
    else
    {
        QIO::cerr << tr("Unable to fetch number of records in table `%1'").arg(tableName) << endl;
    }
    return 0;
}

uint Database::getNumberOfTableColumns(const QString &tableName)
{
    QString selectFieldCountText = QString(
                "SELECT COUNT(*) FROM information_schema.`COLUMNS`"
                "WHERE table_name = '%1'"
                "AND TABLE_SCHEMA = DATABASE()").arg(tableName);
    QSqlQuery selectFieldCount;
    selectFieldCount.prepare(selectFieldCountText);
    selectFieldCount.exec();
    selectFieldCount.next();
    uint fieldCount = selectFieldCount.value(0).toUInt();
    return fieldCount;
}

void Database::getTables(QQueue<TableNode *> &tables)
{
    QSqlQuery fetchTables;
    fetchTables.prepare("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA=DATABASE()");
    if(fetchTables.exec())
    {
        while(fetchTables.next())
        {
            TableNode* item = new TableNode(fetchTables.value(0).toString(),-1);
            tables << item;
        }
    }
}

void Database::getParents(const QQueue<TableNode *> &inputList)
{
    QSqlQuery getParents;
    QFile queryFile(":/data/resources/mysql_fk_fetch.sql");
    if(!queryFile.open(QFile::ReadOnly | QFile::Text))
    {
        QIO::cerr << tr("Unable to open SQL query file") << endl;
        return;
    }
    QString genericQueryText = QString::fromUtf8(queryFile.readAll());
    foreach(TableNode* item, inputList)
    {
        QString queryText = genericQueryText.arg(item->name);
        getParents.prepare(queryText);
        if(getParents.exec())
        {
            while(getParents.next())
            {
                QString tableName = getParents.value(0).toString();
                foreach(TableNode* parentItem, inputList)
                {
                    if(parentItem->name == tableName)
                    {
                        item->referencedTables << parentItem;
                    }
                }
            }
        }
        else
        {
            QIO::cerr << tr("Unable to execute statement") << endl;
            QIO::cerr << getParents.lastError().text() << endl;
            // TODO: throw new exception
        }
    }
}

void Database::sortTables(QQueue<TableNode *> &input, QQueue<TableNode *> &output)
{
    while(! input.isEmpty())
    {
        // Check dependencies
        bool satisfied = true;
        foreach(TableNode* parent, input.head()->referencedTables)
        {
            if(parent->degreeOfFreedom == -1)
            {
                satisfied = false;
                break;
            }
            else
            {

            }
        }
        if(satisfied)
        {
            int max =0;
            foreach(TableNode* parent, input.head()->referencedTables)
                max = (max < parent->degreeOfFreedom+1) ? parent->degreeOfFreedom+1 : max;
            input.head()->degreeOfFreedom = max;
            output.enqueue(input.dequeue());
        }
        else
        {
            input.enqueue(input.dequeue());
        }
    }

}
