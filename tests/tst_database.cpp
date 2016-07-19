#include "tst_database.h"
#include <qtz/data/database.h>
#include <QtTest/QtTest>
#include <QSqlError>

DatabaseTest::DatabaseTest(QObject *parent):
    TestUnit(parent),
    enableMysql(false)
{
}

QStringList DatabaseTest::parseArguments(int argc, char *argv[])
{
    QStringList args = TestUnit::pruneQtTestArguments(argc,argv);
    parseArgumentsHelper(args);
    return args;
}

void DatabaseTest::parseArguments(QStringList &arguments)
{
    parseArguments(arguments);
}

QString DatabaseTest::getMysqlDB()
{
    return mysqlDB;
}

QString DatabaseTest::getMysqlHost()
{
    return mysqlHost;
}

QString DatabaseTest::getMysqlPort()
{
    return mysqlPort;
}

QString DatabaseTest::getMysqlUserName()
{
    return mysqlUserName;
}

QString DatabaseTest::getMysqlPassword()
{
    return mysqlPassword;
}

QString DatabaseTest::getMysqlRootPassword()
{
    return mysqlRootPassword;
}

void DatabaseTest::initTestCase()
{
    if(! (enableMysql)) {
        qDebug("Please specifiy at least one database system to test.");
        qDebug("No database backend has been specified. Exiting cleanly.");
    }
}

void DatabaseTest::connect()
{
    if(enableMysql) {
        Database::getInstance()->setType(Database::Type::MySQL5);
        Database::getInstance()->database()->setHostName(mysqlHost);
        Database::getInstance()->database()->setPort(mysqlPort.toInt());
        Database::getInstance()->database()->setDatabaseName(mysqlDB);
        Database::getInstance()->database()->setUserName(mysqlUserName);
        Database::getInstance()->database()->setPassword(mysqlPassword);
        Database::getInstance()->database()->open();
        QVERIFY(Database::getInstance()->database()->isOpen() &&
                Database::getInstance()->database()->isValid()
               );
    }
    else {
        QVERIFY(false);
    }
}

void DatabaseTest::fillSampleData()
{
    //TODO: Write a cross-platform data filler
    // We suppose data is already filled into database.
    QVERIFY(true);
}

void DatabaseTest::backupByVariant()
{
    //Database::getInstance()->backupByVariant("./BACKUP.DAT");
}

void DatabaseTest::restore()
{
    //    Database::getInstance()->clearDatabase();
    //    Database::getInstance()->restore("./BACKUP.DAT");
}

void DatabaseTest::cleanupTestCase()
{
}

void DatabaseTest::parseArgumentsHelper(QStringList &arguments)
{
    QMutableStringListIterator a(arguments);
    while(a.hasNext()) {
        QString value = a.next();
        if(value == "--enable-mysql") {
            a.remove();
            this->enableMysql = true;
        }
        else if(value == "--disable-mysql") {
            a.remove();
            this->enableMysql = false;
        }
        else if(value == "--mysql-database") {
            a.remove();
            this->mysqlDB = a.next();
            a.remove();
        }
        else if(value == "--mysql-host") {
            a.remove();
            this->mysqlHost = a.next();
            a.remove();
        }
        else if(value == "--mysql-port") {
            a.remove();
            this->mysqlPort = a.next();
            a.remove();
        }
        else if(value == "--mysql-username") {
            a.remove();
            this->mysqlUserName = a.next();
            a.remove();
        }
        else if(value == "--mysql-password") {
            a.remove();
            this->mysqlPassword = a.next();
            a.remove();
        }
        else if(value == "--mysql-root-password") {
            a.remove();
            this->mysqlRootPassword = a.next();
            a.remove();
        }
    }
}
