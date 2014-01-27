#ifndef TST_DATABASE_H
#define TST_DATABASE_H
#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class DatabaseTest : public QObject {
    Q_OBJECT

public:
    DatabaseTest();

private slots:
    void initTestCase();
    void connect();
    void fillSampleData();
    void backupByVariant();
    void restore();
    void cleanupTestCase();
};

#endif // TST_DATABASE_H
