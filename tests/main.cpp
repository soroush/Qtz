#include <QtTest/QtTest>
#include "tst_authprovider.h"
#include "tst_database.h"
#include "tst_gui.h"
#include <QStringList>
#include <QApplication>

#include <QCoreApplication>
#include "testunit.h"

#include <QDebug>
#include <qtz/core/settings.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QStringList testArgs = TestUnit::pruneExtraArguments(argc,argv);
    QStringList args;
    // Database Backend
    DatabaseTest *db_test = new DatabaseTest();
    args = db_test->parseArguments(argc,argv);
    //QTest::qExec(db_test, testArgs);
    // GUI
    Settings::initialize("Phemto Ltd.","QtzTest");
    GuiTest *gui = new GuiTest(db_test);
    QTest::qExec(gui, testArgs);
    /*
    AuthProviderTest *auth_test = new AuthProviderTest();
    auth_test->parseArguments(args);
    QTest::qExec(&auth_test, argc, argv);
    */
    return app.exec();
}
