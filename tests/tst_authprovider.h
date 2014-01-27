#ifndef TST_AUTHPROVIDER_H
#define TST_AUTHPROVIDER_H

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class AuthProviderTest : public QObject {
    Q_OBJECT

public:
    AuthProviderTest();

private slots:
    void initTestCase() {
        qDebug("Preparing Database Connection");
    }
    void myFirstTest() {
        QVERIFY(1 == 1);
    }
    void mySecondTest() {
        QVERIFY(1 != 2);
    }
    void cleanupTestCase() {
        qDebug("called after myFirstTest and mySecondTest");
    }
    void authenticate();
};

#endif // TST_AUTHPROVIDER_H
