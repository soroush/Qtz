#ifndef TST_DATABASE_H
#define TST_DATABASE_H

#include <QtCore/QString>
#include <QtCore/QCoreApplication>
#include "testunit.h"

class DatabaseTest : public TestUnit {
    Q_OBJECT

public:
    DatabaseTest(QObject *parent=nullptr);
    QStringList parseArguments(int argc, char *argv[]);
    void parseArguments(QStringList& arguments);
    QString getMysqlDB();
    QString getMysqlHost();
    QString getMysqlPort();
    QString getMysqlUserName();
    QString getMysqlPassword();
    QString getMysqlRootPassword();


private slots:
    void initTestCase();
    void connect();
    void fillSampleData();
    void backupByVariant();
    void restore();
    void cleanupTestCase();

private:
    bool enableMysql;
    QString mysqlDB;
    QString mysqlHost;
    QString mysqlPort;
    QString mysqlUserName;
    QString mysqlPassword;
    QString mysqlRootPassword;

protected:
    void parseArgumentsHelper(QStringList &arguments);
};

#endif // TST_DATABASE_H
