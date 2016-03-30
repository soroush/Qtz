#include "qio.h"
#include <cstdio>

QTZ_SHARED_EXPORT QTextStream QIO::cout(stdout);
QTZ_SHARED_EXPORT QTextStream QIO::cin(stdin);
QTZ_SHARED_EXPORT QTextStream QIO::cerr(stderr);

QIO::QIO(QObject *parent) :
    QObject(parent)
{
}
