#ifndef TESTUNIT_H
#define TESTUNIT_H

#include <QObject>
#include <QStringList>

class TestUnit : public QObject {
    Q_OBJECT
public:
    TestUnit(QObject *parent=nullptr);
    virtual QStringList parseArguments(int argc, char *argv[]) = 0;
    virtual void parseArguments(QStringList& arguments) = 0;
    static QStringList pruneExtraArguments(int argc, char *argv[]);
    static QStringList pruneQtTestArguments(int argc, char *argv[]);
};

#endif // TESTUNIT_H
