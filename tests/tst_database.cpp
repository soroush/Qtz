#include "tst_database.h"
#include <qtz/data/database.h>

DatabaseTest::DatabaseTest(  )
{
}

void DatabaseTest::initTestCase()
{
#ifdef MYSQL_TEST
#if !defined(MYSQL_PASSWORD) || !defined(MYSQL_USERNAME)
#error Please specify MySQL database connection information.
#endif
#endif
}

void DatabaseTest::connect()
{
#ifdef MYSQL_TEST
    Database::getInstance()->setType(Database::Type::MySQL5);
    Database::getInstance()->database()->setHostName(MYSQL_HOST);
    Database::getInstance()->database()->setPort(MYSQL_PORT);
    Database::getInstance()->database()->setDatabaseName(MYSQL_DB);
    Database::getInstance()->database()->setUserName(MYSQL_USERNAME);
    Database::getInstance()->database()->setPassword(MYSQL_PASSWORD);
    QVERIFY(Database::getInstance()->database()->open());
#endif
}

void DatabaseTest::fillSampleData()
{
    //TODO: Write a cross-platform data filler
    // We suppose data is already filled into database.
    QVERIFY(true);
}

void DatabaseTest::backupByVariant()
{
    Database::getInstance()->backupByVariant("./BACKUP.DAT");
}

void DatabaseTest::restore()
{
    Database::getInstance()->clearDatabase();
//    Database::getInstance()->restore("./BACKUP.DAT");
}

void DatabaseTest::cleanupTestCase()
{
}
