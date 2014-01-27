#include <QtTest/QtTest>
#include "tst_authprovider.h"
#include "tst_database.h"

int main(int argc, char **argv)
{
    DatabaseTest db_test;
    QTest::qExec(&db_test, argc, argv);
    AuthProviderTest auth_test;
//    QTest::qExec(&auth_test, argc, argv);
    return 0;
}
