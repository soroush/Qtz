#include "testunit.h"

TestUnit::TestUnit(QObject *parent):
    QObject(parent)
{
}

QStringList TestUnit::pruneExtraArguments(int argc, char *argv[])
{
    QStringList args;
    for (int i = 1; i < argc; ++i) {
        args << argv[i];
    }
    QStringList qtTestArgs;
    QMutableStringListIterator a(args);
    while(a.hasNext()) {
        QString value = a.next();
        if(! value.startsWith("--")) {
            qtTestArgs << value;
        }
        else {
            while(a.hasNext()) {
                a.next();
                a.remove();
            }
        }
    }
    return qtTestArgs;
}

QStringList TestUnit::pruneQtTestArguments(int argc, char *argv[])
{
    QStringList args;
    for (int i = 1; i < argc; ++i) {
        args << argv[i];
    }
    QStringList extraArgs;
    QMutableStringListIterator a(args);
    while(a.hasNext()) {
        QString value = a.next();
        if(value.startsWith("--")) {
            extraArgs << value;
            while(a.hasNext()) {
                extraArgs << a.next();
            }
        }
    }
    return extraArgs;
}
