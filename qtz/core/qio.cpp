#include "qio.h"
#include <cstdio>

QTextStream QIO::cout(stdout);
QTextStream QIO::cin(stdin);
QTextStream QIO::cerr(stderr);

QIO::QIO(QObject *parent) :
    QObject(parent)
{
}
