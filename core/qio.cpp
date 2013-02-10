#include "qio.h"

QTextStream QIO::cout;
QTextStream QIO::cin;
QTextStream QIO::cerr;

QIO::QIO(QObject *parent) :
    QObject(parent)
{
}
